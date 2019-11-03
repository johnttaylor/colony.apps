/*-----------------------------------------------------------------------------
* This file is part of the Colony.Apps Project.  The Colony.Apps Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.apps/license.txt
*
* Copyright (c) 2015 - 2019 John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "IndoorHeating.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Math/real.h"


#define SECT_ "Storm::Component::Equipment"

using namespace Storm::Component::Equipment;

//////////////////////////////////////////////////////////////
IndoorHeating::IndoorHeating( StageApi& firstStageIdHeating, StageApi& secondStageIdHeating, StageApi& thirdStageIdHeating )
    : m_1Stage( firstStageIdHeating )
    , m_2Stage( secondStageIdHeating )
    , m_3Stage( thirdStageIdHeating )
{
}

//////////////////////////////////////////////////////////////
bool IndoorHeating::executeActive( Storm::Type::SystemType systemType, Args_T& args ) noexcept
{
    switch ( systemType )
    {
        // 1st Stage logic
    case Storm::Type::SystemType::eAC1_FURN1:
    case Storm::Type::SystemType::eNONE_FURN1:
        singleStage( systemType, args );

        // 'Run' the stage(s)
        m_1Stage.execute( args );
        break;

        // 2 stage logic
    case Storm::Type::SystemType::eAC1_FURN2:
    case Storm::Type::SystemType::eNONE_FURN2:
        if ( !secondStage( systemType, args ) )
        {
            singleStage( systemType, args );
        }

        // 'Run' the stage(s)
        m_1Stage.execute( args );
        m_2Stage.execute( args );
        break;

        // 3 stage logic
    case Storm::Type::SystemType::eAC1_FURN3:
    case Storm::Type::SystemType::eNONE_FURN3:
        if ( !thirdStage( systemType, args ) )
        {
            if ( !secondStage( systemType, args ) )
            {
                // 1st Stage logic
                singleStage( systemType, args );
            }
        }

        // 'Run' the stage(s)
        m_1Stage.execute( args );
        m_2Stage.execute( args );
        m_3Stage.execute( args );
        break;

    default:
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "IndoorHeating::executeActive: Unsupported SystemType (%d)", systemType ) );
        return false;
    }

    return true;
}

bool IndoorHeating::executeOff( Storm::Type::SystemType systemType, Args_T& args ) noexcept
{
    m_1Stage.requestModeToOff();
    m_2Stage.requestModeToOff();
    m_3Stage.requestModeToOff();
    return true;
}

bool IndoorHeating::start( Cpl::System::ElapsedTime::Precision_T intervalTime ) noexcept
{
    reset();
    return true;
}

void IndoorHeating::reset() noexcept
{
    // initialize Stage object(s)
    m_1Stage.requestModeToOff();
    m_2Stage.requestModeToOff();
    m_3Stage.requestModeToOff();
}

//////////////////////////////////////////////////////////////
bool IndoorHeating::singleStage( Storm::Type::SystemType systemType, Args_T& args ) noexcept
{
    // If the system was previously off AND there is sufficient load to turn on
    if ( args.systemOn == false && args.pvOut > OPTION_STORM_COMPONENT_EQUIPMENT_INDOOR_HEATING_TURN_ON_THRESHOLD )
    {
        // Has the system met the minimum indoor heater off time?
        Cpl::System::ElapsedTime::Precision_T minOffTime = { OPTION_STORM_MIN_INDOOR_HEATER_OFF_TIME_SEC, 0 };
        if ( Cpl::System::ElapsedTime::expiredPrecision( args.equipmentBeginTimes.indoorUnitBeginOffTime, minOffTime, args.currentInterval ) )
        {
            m_1Stage.requestOn( args );
        }

        return true;
    }

    // stage 1 is the active stage AND there is no longer sufficient load requiring conditioning 
    else if ( m_1Stage.isActive() && args.pvOut <= OPTION_STORM_COMPONENT_EQUIPMENT_INDOOR_HEATING_TURN_OFF_THRESHOLD )
    {
        // Am I in an off-cycle OR has the system met the minimum 1st stage on cycle time?
        Cpl::System::ElapsedTime::Precision_T minOnTime = { args.comfortConfig.heating[0].minOnTime, 0 };
        if ( m_1Stage.isOffCycle() || Cpl::System::ElapsedTime::expiredPrecision( args.cycleInfo.beginOnTime, minOnTime, args.currentInterval ) )
        {
            m_1Stage.requestOff( args );
        }

        return true;
    }

    // Return false because no 'logic was tripped'
    return false;
}

bool IndoorHeating::secondStage( Storm::Type::SystemType systemType, Args_T& args ) noexcept
{
    // If the system was previously off AND there is sufficient load to turn on WITH THE SECOND STAGE
    if ( args.systemOn == false && args.pvOut > OPTION_STORM_COMPONENT_EQUIPMENT_INDOOR_HEATING_TURN_ON_2ND_STAGE_THRESHOLD )
    {
        // Has the system met the minimum indoor heater off time?
        Cpl::System::ElapsedTime::Precision_T minOffTime = { OPTION_STORM_MIN_INDOOR_HEATER_OFF_TIME_SEC, 0 };
        if ( Cpl::System::ElapsedTime::expiredPrecision( args.equipmentBeginTimes.indoorUnitBeginOffTime, minOffTime, args.currentInterval ) )
        {
            m_1Stage.requestOn( args );
            m_1Stage.requestAsSupplement( args, m_2Stage );
        }

        return true;
    }

    // cycling the 1st stage AND there is sufficient load to turn on with the SECOND stage
    else if ( m_1Stage.isActive() && args.pvOut > OPTION_STORM_COMPONENT_EQUIPMENT_INDOOR_HEATING_TURN_ON_2ND_STAGE_THRESHOLD )
    {
        // The first stage is on OR I am in off cycle and have met the minimum equipment off time?
        Cpl::System::ElapsedTime::Precision_T minOffTime = { OPTION_STORM_MIN_INDOOR_HEATER_OFF_TIME_SEC, 0 };
        if ( m_1Stage.isOnCycle() ||
            ( m_1Stage.isOffCycle() && Cpl::System::ElapsedTime::expiredPrecision( args.equipmentBeginTimes.indoorUnitBeginOffTime, minOffTime, args.currentInterval ) ) )
        {
            m_1Stage.requestAsSupplement( args, m_2Stage );
        }

        return true;
    }

    // Cycling the 2nd stage AND there is no longer sufficient load for the 2nd stage
    else if ( m_2Stage.isActive() && args.pvOut <= OPTION_STORM_COMPONENT_EQUIPMENT_INDOOR_HEATING_TURN_OFF_2ND_STAGE_THRESHOLD )
    {
        // Am I in an off-cycle OR have met the minimum 2nd stage on cycle time?
        Cpl::System::ElapsedTime::Precision_T minOnTime = { args.comfortConfig.heating[1].minOnTime, 0 };
        if ( m_2Stage.isOffCycle() || Cpl::System::ElapsedTime::expiredPrecision( args.cycleInfo.beginOnTime, minOnTime, args.currentInterval ) )
        {
            m_2Stage.requestOff( args, &m_1Stage );
        }

        return true;
    }

    // Return false because no 'logic was tripped'
    return false;
}

bool IndoorHeating::thirdStage( Storm::Type::SystemType systemType, Args_T& args ) noexcept
{
    // If the system was previously off AND there is sufficient load to turn on WITH THE THIRD STAGE
    if ( args.systemOn == false && args.pvOut > OPTION_STORM_COMPONENT_EQUIPMENT_INDOOR_HEATING_TURN_ON_3RD_STAGE_THRESHOLD )
    {
        // Has the system met the minimum indoor heater off time?
        Cpl::System::ElapsedTime::Precision_T minOffTime = { OPTION_STORM_MIN_INDOOR_HEATER_OFF_TIME_SEC, 0 };
        if ( Cpl::System::ElapsedTime::expiredPrecision( args.equipmentBeginTimes.indoorUnitBeginOffTime, minOffTime, args.currentInterval ) )
        {
            // Turn on the First stage and initiate the transition to the second stage (can't transition to the 3rd stage until the 2nd becomes 'active')
            m_1Stage.requestOn( args );
            m_1Stage.requestAsSupplement( args, m_2Stage );
        }

        return true;
    }

    // cycling the 1st stage AND there is sufficient load to turn on with the THIRD stage
    else if ( m_1Stage.isActive() && args.pvOut > OPTION_STORM_COMPONENT_EQUIPMENT_INDOOR_HEATING_TURN_ON_3RD_STAGE_THRESHOLD )
    {
        // The first stage is on OR I am in off cycle and have met the minimum equipment off time?
        Cpl::System::ElapsedTime::Precision_T minOffTime = { OPTION_STORM_MIN_INDOOR_HEATER_OFF_TIME_SEC, 0 };
        if ( m_1Stage.isOnCycle() ||
            ( m_1Stage.isOffCycle() && Cpl::System::ElapsedTime::expiredPrecision( args.equipmentBeginTimes.indoorUnitBeginOffTime, minOffTime, args.currentInterval ) ) )
        {
            // Initiate the transition to the second stage (can't transition to the 3rd stage until the 2nd becomes 'active')
            m_1Stage.requestAsSupplement( args, m_2Stage );
        }

        return true;
    }

    // cycling the 2nd stage AND there is sufficient load to turn on with the THIRD stage
    else if ( m_2Stage.isActive() && args.pvOut > OPTION_STORM_COMPONENT_EQUIPMENT_INDOOR_HEATING_TURN_ON_3RD_STAGE_THRESHOLD )
    {
        // The first stage is on OR I am in off cycle and have met the minimum equipment off time?
        Cpl::System::ElapsedTime::Precision_T minOffTime = { OPTION_STORM_MIN_INDOOR_HEATER_OFF_TIME_SEC, 0 };
        if ( m_2Stage.isOnCycle() ||
            ( m_2Stage.isOffCycle() && Cpl::System::ElapsedTime::expiredPrecision( args.equipmentBeginTimes.indoorUnitBeginOffTime, minOffTime, args.currentInterval ) ) )
        {
            // Initiate the transition to the third stage
            m_2Stage.requestAsSupplement( args, m_3Stage );
        }

        return true;
    }

    // Cycling the 3rd stage AND there is no longer sufficient load for the 3rd stage
    else if ( m_3Stage.isActive() && args.pvOut <= OPTION_STORM_COMPONENT_EQUIPMENT_INDOOR_HEATING_TURN_OFF_3RD_STAGE_THRESHOLD )
    {
        // Am I in an off-cycle OR have met the minimum 3rd stage on cycle time?
        Cpl::System::ElapsedTime::Precision_T minOnTime = { args.comfortConfig.heating[2].minOnTime, 0 };
        if ( m_3Stage.isOffCycle() || Cpl::System::ElapsedTime::expiredPrecision( args.cycleInfo.beginOnTime, minOnTime, args.currentInterval ) )
        {
            m_3Stage.requestOff( args, &m_2Stage );
        }

        return true;
    }

    // Return false because no 'logic was tripped'
    return false;
}