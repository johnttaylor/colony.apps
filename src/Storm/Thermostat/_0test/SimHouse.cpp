/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2019  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "SimHouse.h"
#include "ModelPoints.h"
#include "Storm/Thermostat/ModelPoints.h"

#ifndef OPTION_SIM_TIMING_MS
#define OPTION_SIM_TIMING_MS        1000
#endif

#define RESISTANCE_NO_CAPACITY      50
#define RESISTANCE_COOLING_CAPCITY  (RESISTANCE_NO_CAPACITY*2.0/3.0)
#define RESISTANCE_HEATING_CAPCITY  (RESISTANCE_COOLING_CAPCITY*3.0)

//////////////////////
SimHouse::SimHouse()
    : EventLoop( OPTION_SIM_TIMING_MS )     // Wake up once a second
    , m_sim( OPTION_SIM_TIMING_MS/1000.0    // Convert to seconds
             , 70.0                         // ODT
             , 120.0                        // max odt
             , -20.0                        // min odt
             , 97.0                         // ODT Cooling load rating
             , -10.0                        // ODT Heating load rating
             , RESISTANCE_NO_CAPACITY       // systemEnvResistance
             , RESISTANCE_COOLING_CAPCITY   // systemCoolingEnvResistance
             , RESISTANCE_HEATING_CAPCITY ) // systemHeatingEnvResistance
{
}

void SimHouse::appRun()
{
    startEventLoop();
    unsigned long timeMark = Cpl::System::ElapsedTime::milliseconds();

    bool run = true;
    while ( run )
    {
        run = waitAndProcessEvents();
        if ( run )
        {
            unsigned long timeNow = Cpl::System::ElapsedTime::milliseconds();
            while ( Cpl::System::ElapsedTime::deltaMilliseconds( timeMark, timeNow ) >= OPTION_SIM_TIMING_MS )
            {
                timeMark += OPTION_SIM_TIMING_MS;
                executeSimulation();
            }
        }
    }
}

void SimHouse::executeSimulation()
{
    double                          odt;
    Storm::Type::SystemConfig_T     sysCfg;
    Storm::Type::HvacRelayOutputs_T relays;
    bool                            simEnabled;
    if ( Cpl::Dm::ModelPoint::IS_VALID( mp_outdoorTemp.read( odt ) ) == true &&
         Cpl::Dm::ModelPoint::IS_VALID( mp_systemConfig.read( sysCfg ) ) == true &&
         Cpl::Dm::ModelPoint::IS_VALID( mp_relayOutputs.read( relays ) ) == true &&
         Cpl::Dm::ModelPoint::IS_VALID( mp_houseSimEnabled.read( simEnabled ) ) == true )
    {
        double capacity = 0.0;
        bool   cooling  = true;

        // Cooling capacity
        if ( sysCfg.currentOpMode == Storm::Type::OperatingMode::eCOOLING )
        {
            double stageCapacity = sysCfg.numCompressorStages / ( ( double) OPTION_STORM_MAX_COMPRESSOR_COOLING_STAGES );
            capacity += relays.y1 ? stageCapacity : 0.0;
            if ( sysCfg.numCompressorStages > 1 )
            {
                capacity += relays.y2 ? stageCapacity : 0.0;
            }
        }

        // Compressor + Indoor Heating capacity
        else if ( sysCfg.currentOpMode == Storm::Type::OperatingMode::eHEATING )
        {
            cooling = false;

            // HeatPump with Electric heat
            if ( sysCfg.indoorUnitType == Storm::Type::IduType::eAIR_HANDLER )
            {
                double stageCapacity = ( sysCfg.numCompressorStages + sysCfg.numIndoorStages ) / ( ( double) OPTION_STORM_MAX_HEATING_STAGES );
                capacity += relays.y1? stageCapacity: 0.0;
                if ( sysCfg.numCompressorStages > 1 )
                {
                    capacity += relays.y2 ? stageCapacity : 0.0;
                }
                if ( sysCfg.numIndoorStages > 0 )
                {
                    capacity += relays.w1 ? stageCapacity : 0.0;
                }
                if ( sysCfg.numIndoorStages > 1 )
                {
                    capacity += relays.w2 ? stageCapacity : 0.0;
                }
                if ( sysCfg.numIndoorStages > 2 )
                {
                    capacity += relays.w3 ? stageCapacity : 0.0;
                }
            }

            // DUAL fuel
            else  if ( sysCfg.indoorUnitType == Storm::Type::IduType::eFURNACE )
            {
                // TODO
            }
        }

        // Indoor Heating capacity
        else if ( sysCfg.currentOpMode == Storm::Type::OperatingMode::eID_HEATING )
        {
            cooling = false;
            double stageCapacity = sysCfg.numIndoorStages / ( ( double) STORM_MAX_INDOOR_STAGES );
            capacity += relays.w1 ? stageCapacity : 0.0;
            if ( sysCfg.numIndoorStages > 1 )
            {
                capacity += relays.w2 ? stageCapacity : 0.0;
            }
            if ( sysCfg.numIndoorStages > 2 )
            {
                capacity += relays.w3 ? stageCapacity : 0.0;
            }
        }

        // Run the simulation
        double idt = m_sim.tick( odt, capacity, cooling );

        // Update the Thermostat (when enabled )
        if ( simEnabled )
        {
            mp_primaryRawIdt.write( ( float) idt );
        }
    }
}
