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


#include "FanControl.h"
#include "Cpl/System/Trace.h"

#define SECT_   "Storm::Component::FanControl"


/// Namespaces
using namespace Storm::Component;



///////////////////////////////
FanControl::FanControl( struct Input_T ins, struct Output_T outs )
    : m_in( ins )
    , m_out( outs )
{
}

bool FanControl::start( Cpl::System::ElapsedTime::Precision_T intervalTime )
{
    // Initialize parent class
    return Base::start( intervalTime );
}


///////////////////////////////
bool FanControl::execute( Cpl::System::ElapsedTime::Precision_T currentTick,
                          Cpl::System::ElapsedTime::Precision_T currentInterval )
{
    CPL_SYSTEM_TRACE_FUNC( SECT_ );

    //--------------------------------------------------------------------------
    // Pre-Algorithm processing
    //--------------------------------------------------------------------------

    // Get my inputs
    Storm::Type::FanMode            fanMode        = Storm::Type::FanMode::eAUTO;
    Storm::Type::SystemType         systemType     = Storm::Type::SystemType::eUNDEFINED;
    uint32_t                        forceOffRefCnt = 0;
    Storm::Type::VirtualOutputs_T   outputs        = { 0, };
    Storm::Type::EquipmentTimes_T   equipTimes     = { 0, };
    Storm::Type::ComfortConfig_T    config         = { 0, };
    Storm::Dm::MpIduConfig::Data    iduConfig      = { 0, };
    int8_t                          validMode      = m_in.fanMode.read( fanMode );
    int8_t                          validSystem    = m_in.systemType.read( systemType );
    int8_t                          validOutputs   = m_in.vOutputs.read( outputs );
    int8_t                          validEquipment = m_in.equipmentBeginTimes.read( equipTimes );
    int8_t                          validComfort   = m_in.comfortConfig.read( config );
    int8_t                          validForceOff  = m_in.systemForcedOffRefCnt.read( forceOffRefCnt );
    int8_t                          validIduCfg    = m_in.iduConfig.read( iduConfig );
    int                             newFanOutput   = -1;
    if ( Cpl::Dm::ModelPoint::IS_VALID( validMode ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validSystem ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validOutputs ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validEquipment ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validComfort ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validForceOff ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validIduCfg ) == false )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "FanControl::execute. One or more invalid MPs (FanMode=%d, systemType=%d, vOutputs=%d equipTimes=%d, comfort=%d, forceOffCnt=%d, idCfg=%d", validMode, validSystem, validOutputs, validEquipment, validComfort, validForceOff, validIduCfg ) );

        // Silently do nothing! (Not sure what else make sense??  Suggestions?)
        return true;
    }


    //--------------------------------------------------------------------------
    // Algorithm processing
    //--------------------------------------------------------------------------

    /* NOTE: If the Indoor fan is being 'actively' controlled by the Cooling/Heating
             equipment - it will be set on/off as needed.  Also, if the operating
             mode is OFF, the indoor fan will have already been set to the off
             state.  All this means - no action is required in the FanControl
             component to turn the indoor fan OFF when the user transitions from
             FanContinous to Fan Auto
    */

    // Force the fan off is system is being forced off
    if ( forceOffRefCnt > 0 )
    {
        newFanOutput = STORM_DM_MP_VIRTUAL_OUTPUTS_OFF;
    }
    
    // System allowed to run
    else
    {
        // Simple Fan Continuous - Force the Fan ON
        if ( fanMode == +Storm::Type::FanMode::eCONTINUOUS )
        {
            newFanOutput = iduConfig.hasVspMotor ? OPTION_STORM_COMPONENT_FAN_CONTROL_CONTINUOUS_SPEED : STORM_DM_MP_VIRTUAL_OUTPUTS_ON;
        }
    }
    
    //--------------------------------------------------------------------------
    // Post-Algorithm processing
    //--------------------------------------------------------------------------

    // All done (update fan output)
    if ( newFanOutput >= 0 )
    {
        m_out.vOutputs.setIndoorFanOutput( newFanOutput );
    }


    // If I get here -->everything worked!
    return true;
}


