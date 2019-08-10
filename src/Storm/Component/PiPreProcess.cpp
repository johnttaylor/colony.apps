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
    bool                                  badInputs         = false;
    float                                 idt               = 0.0F; // Default to 'bad value'
    float                                 coolSetpoint      = 0.0F;
    float                                 heatSetpoint      = 0.0F;
    bool                                  modeChanged       = false;
    Storm::Type::OperatingMode            mode              = Storm::Type::OperatingMode::eOFF;
    Storm::Dm::MpIduConfig::Data          iduConfig;
    Storm::Dm::MpOduConfig::Data          oduConfig;
    int8_t                                validIdt          = m_in.activeIdt.read( idt );
    int8_t                                validMode         = m_in.operatingMode.read( mode );
    int8_t                                validSetpoints    = m_in.setpoints.read( coolSetpoint, heatSetpoint );
    int8_t                                validIduConfig    = m_in.iduConfig.read( iduConfig );
    int8_t                                validOduConfig    = m_in.oduConfig.read( oduConfig );
    if ( Cpl::Dm::ModelPoint::IS_VALID( validIdt ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validMode ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validSetpoints ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validIduConfig ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validOduConfig ) == false )
    {
        badInputs = true;
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "PiPreProcess::execute. One or more invalid MPs (idt=%d, mode=%d, setpts=%d, idcfg=%d, odcfg=%d", validIdt, validMode, validSetpoints, validIduConfig, validOduConfig ) );
    }

    // Default the output value(s)
    bool  setpointChanged   = false;
    float deltaIdtError     = 0;
    float setpoitnDelta     = 0;
    float newActiveSetpoint = coolSetpoint;
    float piGain            = OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_NORMAL_GAIN;
    float piReset           = OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_NORMAL_RESET;
    float maxPvOut
    //--------------------------------------------------------------------------
    // Algorithm processing
    //--------------------------------------------------------------------------

    // Use the current operating mode to calculate the delta error
    switch ( mode )
    {
        // COOLING
    case Storm::Type::OperatingMode::eCOOLING:
        deltaIdtError = idt - coolSetpoint;
        outputs.m_maxOutValue = calcPiCoolingMaxOut( inputs );
        break;


        // HEATING
    case Storm::Type::OMode::eHEATING:
        newActiveSetpoint     = m_in.m_heatingSetpoint;
        outputs.m_deltaError  = m_in.m_heatingSetpoint - m_in.m_idt;
        outputs.m_maxOutValue = calcPiHeatingMaxOut( inputs );
        if ( m_in.m_heatingFastPiEnabled )
        {
            outputs.m_gain        = m_in.m_heatingGain1;
            outputs.m_resetTime   = m_in.m_heatingResetTime1;
        }
        else
        {
            outputs.m_gain        = m_in.m_heatingGain0;
            outputs.m_resetTime   = m_in.m_heatingResetTime0;
        }
        break;


        // OFF mode (and any invalid mode settings)
    default:
        newActiveSetpoint = INVALID_SETPOINT_;
        break;
    }


    // Trap a change in the active setpoint (but NOT when there is a mode change)
    if ( m_in.m_opModeChanged.isPulsed() == false )
    {
        outputs.m_deltaSetpoint = fabs( newActiveSetpoint - m_prevActiveSetpoint );
        if ( Cpl::Math::areFloatsEqual( outputs.m_deltaSetpoint, 0.0 ) == false )
        {
            outputs.m_setpointChanged.pulse();
        }
    }

    // Update my cached active setpoint
    m_prevActiveSetpoint = newActiveSetpoint;


    //--------------------------------------------------------------------------
    // Post-Algorithm processing
    //--------------------------------------------------------------------------

    m_out.setpointChanged.write( false );
    m_out.idtDeltaError.write( 0 );
    m_out.setpointDelta.write( 0 );
    m_out.piConstants.write( OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_NORMAL_GAIN, OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_NORMAL_RESET );
    m_out.activeSetpoint.write( coolSetpoint );

    // All done -->set the outputs
    if ( !setOutputs( outputs ) )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "[%p] Failed setOutputs", this ) );
        return false;
    }

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
