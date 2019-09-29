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


#include "Control.h"
#include "Cpl/System/Trace.h"

#define SECT_   "Storm::Component::Control"


/// Namespaces
using namespace Storm::Component;



///////////////////////////////
Control::Control( Control::Equipment& controlLogic, struct Input_T ins, struct Output_T outs )
    : m_in( ins )
    , m_out( outs )
    , m_equipment( controlLogic )
{
}

bool Control::start( Cpl::System::ElapsedTime::Precision_T intervalTime )
{
    // Initialize parent class
    bool result = Base::start( intervalTime );

    // Initialize the control logic
    return m_equipment.start( intervalTime ) & result;
}


///////////////////////////////
bool Control::execute( Cpl::System::ElapsedTime::Precision_T currentTick,
                       Cpl::System::ElapsedTime::Precision_T currentInterval )
{
    CPL_SYSTEM_TRACE_FUNC( SECT_ );

    // Get my inputs
    Storm::Type::OperatingMode      operatingMode  = Storm::Type::OperatingMode::eOFF;
    Storm::Type::SystemType         systemType     = Storm::Type::SystemType::eUNDEFINED;
    float                           pvOut          = 0.0F;
    Storm::Type::VirtualOutputs_T   vOutputs       = { 0, };
    Storm::Type::EquipmentTimes_T   equipmentTimes = { 0, };
    int8_t                          validMode      = m_in.operatingMode.read( operatingMode );
    int8_t                          validSystem    = m_in.systemType.read( systemType );
    int8_t                          validPvOut     = m_in.pvOut.read( pvOut );
    int8_t                          validOutputs   = m_in.vOutputs.read( vOutputs );
    int8_t                          validEquipment = m_in.equipmentBeginTimes.read( equipmentTimes );
    if ( Cpl::Dm::ModelPoint::IS_VALID( validMode ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validSystem ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validOutputs ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validEquipment ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validPvOut ) == false )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Control::execute. One or more invalid MPs (operatingMode=%d, systemType=%d, pvOut=%d, vOutputs=%d equipTimes=%d", validMode, validSystem, validPvOut, validOutputs, validEquipment ) );

        // Silently do nothing! (Not sure what else make sense??  Suggestions?)
        return true;
    }

    // Default the output values (except for vOutputs -->use previous/current value)
    Storm::Type::CycleInfo_T cycleInfo = { 0, };
    bool                     systemOn  = false;


    // Call Equipment/Control logic (when there is a match in operating mode)
    if ( operatingMode == m_equipment.getOperatingMode() )
    {
        // Execute the control logic 
        if ( m_equipment.executeActive( currentTick, currentInterval, pvOut, systemType, equipmentTimes, vOutputs, cycleInfo, systemOn ) == false )
        {
            // Put the outputs into a "all offstate" if there is failure 
            m_out.vOutputs.setSafeAllOff();
            return false;
        }

        // Set my outputs
        m_out.vOutputs.write( vOutputs );
        m_out.cycleInfo.write( cycleInfo );
        m_out.systemOn.write( systemOn );
    }

    // Non-match operating mode
    else
    {
        return m_equipment.executeOff( currentTick, currentInterval );
    }

    // If I get here -->everything worked!
    return true;
}


