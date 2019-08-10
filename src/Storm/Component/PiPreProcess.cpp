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


#include "PiPreProcess.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Math/real.h"


#define SECT_               "Storm::Component::PiPreProcess"

#define INVALID_SETPOINT_   9999.0



/// Namespaces
using namespace Storm::Component;


///////////////////////////////
PiPreProcess::PiPreProcess( struct Input_T ins, struct Output_T outs )
    : m_in( ins )
    , m_out( outs )
{
}


bool PiPreProcess::start( Cpl::System::ElapsedTime::Precision_T intervalTime )
{
    // Initialize my data
    m_prevActiveSetpoint = INVALID_SETPOINT_;

    // Initialize parent class
    return Base::start( intervalTime );
}


///////////////////////////////
bool PiPreProcess::execute( Cpl::System::ElapsedTime::Precision_T currentTick,
                            Cpl::System::ElapsedTime::Precision_T currentInterval
)
{
    CPL_SYSTEM_TRACE_FUNC( SECT_ );

    //--------------------------------------------------------------------------
    // Pre-Algorithm processing   
    //--------------------------------------------------------------------------

    // Get my inputs
    float                                 idt               = 0.0F; // Default to 'bad value'
    float                                 coolSetpoint      = 0.0F;
    float                                 heatSetpoint      = 0.0F;
    bool                                  modeChanged       = false;
    Storm::Type::OperatingMode            mode              = Storm::Type::OperatingMode::eOFF;
    Storm::Dm::MpIduConfig::Data          iduConfig;
    Storm::Dm::MpOduConfig::Data          oduConfig;
    int8_t                                validIdt          = m_in.activeIdt.read( idt );
    int8_t                                validMode         = m_in.operatingMode.read( mode );
    int8_t                                validModeChanged  = m_in.operatingModeChange.read( modeChanged );
    int8_t                                validSetpoints    = m_in.setpoints.read( coolSetpoint, heatSetpoint );
    int8_t                                validIduConfig    = m_in.iduConfig.read( iduConfig );
    int8_t                                validOduConfig    = m_in.oduConfig.read( oduConfig );
    if ( Cpl::Dm::ModelPoint::IS_VALID( validIdt ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validMode ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validModeChanged ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validSetpoints ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validIduConfig ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validOduConfig ) == false )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "PiPreProcess::execute. One or more invalid MPs (idt=%d, mode=%d, modechg=%d,setpts=%d, idcfg=%d, odcfg=%d", validIdt, validMode, validModeChanged, validSetpoints, validIduConfig, validOduConfig ) );
    
        // Force the operating mode to off if I am missing one or more inputs values
        mode = Storm::Type::OperatingMode::eOFF;
    }

    // Default the output value(s)
    bool  setpointChanged   = false;
    float deltaIdtError     = 0;
    float setpointDelta     = 0;
    float newActiveSetpoint = coolSetpoint;
    float piGain            = OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_NORMAL_GAIN;
    float piReset           = OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_NORMAL_RESET;
    float maxPvOut          = OPTION_STORM_DM_MP_PI_CONSTANTS_MAX_PV_OUT;

    //--------------------------------------------------------------------------
    // Algorithm processing
    //--------------------------------------------------------------------------

    // Use the current operating mode to calculate the delta error
    switch ( mode )
    {
        // COOLING
    case Storm::Type::OperatingMode::eCOOLING:
        deltaIdtError = idt - coolSetpoint;
        maxPvOut      = calcPiCoolingMaxOut( oduConfig.numStages );
        break;

        // HEATING
    case Storm::Type::OperatingMode::eHEATING:
        newActiveSetpoint = heatSetpoint;
        deltaIdtError     = heatSetpoint - idt;
        maxPvOut          = calcPiHeatingMaxOut( iduConfig.numHeatingStages, Storm::Type::IduType::_from_integral_unchecked( iduConfig.type ), oduConfig.numStages, Storm::Type::OduType::_from_integral_unchecked( oduConfig.type)  );
        piGain            = OPTION_STORM_DM_MP_PI_CONSTANTS_HEATING_NORMAL_GAIN;
        piReset           = OPTION_STORM_DM_MP_PI_CONSTANTS_HEATING_NORMAL_RESET;
        break;

        // OFF mode (and any invalid mode settings)
    default:
        newActiveSetpoint = INVALID_SETPOINT_;
        break;
    }

    // Trap a change in the active setpoint (but NOT when there is a mode change)
    if ( modeChanged == false )
    {
        setpointDelta = fabs( newActiveSetpoint - m_prevActiveSetpoint );
        if ( Cpl::Math::areFloatsEqual( setpointDelta, 0.0 ) == false )
        {
            setpointChanged = true;
        }
    }

    // Update my cached active setpoint
    m_prevActiveSetpoint = newActiveSetpoint;


    //--------------------------------------------------------------------------
    // Post-Algorithm processing
    //--------------------------------------------------------------------------

    // Set my outputs
    m_out.setpointChanged.write( setpointChanged );
    m_out.idtDeltaError.write( deltaIdtError );
    m_out.setpointDelta.write( setpointDelta );
    m_out.piConstants.write( piGain, piReset, maxPvOut );
    m_out.activeSetpoint.write( newActiveSetpoint );

    // If I get here -->everything worked!
    return true;
}




///////////////////////////////
float PiPreProcess::calcPiCoolingMaxOut( unsigned numCoolingStages )
{
    return numCoolingStages * OPTION_STORM_COMPONENT_PI_PREPROCESS_COOLING_LV_PER_STAGE;
}

float PiPreProcess::calcPiHeatingMaxOut( unsigned numIndoorHeatingStage, Storm::Type::IduType iduType, unsigned numOutdoorHeatingStages, Storm::Type::OduType oduType )
{
    return numIndoorHeatingStage * OPTION_STORM_COMPONENT_PI_PREPROCESS_HEATING_LV_PER_STAGE;
}
