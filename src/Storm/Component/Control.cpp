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
    , m_sysCfgSequenceNumber( Cpl::Dm::ModelPoint::SEQUENCE_NUMBER_UNKNOWN )
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
    Equipment::Args_T               args;
    int8_t                          validSystem    = m_in.systemConfig.read( args.systemConfig );
    int8_t                          validPvOut     = m_in.pvOut.read( args.pvOut );
    int8_t                          validOutputs   = m_in.vOutputs.read( args.vOutputs );
    int8_t                          validEquipment = m_in.equipmentBeginTimes.read( args.equipmentBeginTimes );
    int8_t                          validSystemOn  = m_in.systemOn.read( args.systemOn );
    int8_t                          validCycleInfo = m_in.cycleInfo.read( args.cycleInfo );
    if ( Cpl::Dm::ModelPoint::IS_VALID( validSystem ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validOutputs ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validEquipment ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validSystemOn ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validCycleInfo ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validPvOut ) == false )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Control::execute. One or more invalid MPs (system=%d, pvOut=%d, vOutputs=%d equipTimes=%d, systemOn=%d, cycle=%d", validSystem, validPvOut, validOutputs, validEquipment, validSystemOn, validCycleInfo ) );

        // Silently do nothing! (Not sure what else make sense??  Suggestions?)
        return true;
    }

    // Housekeeping
    args.currentInterval = currentInterval;
    args.currentTick     = currentTick;

    // Reset the equipment if the system configuration/type was changed
    uint16_t sysSeqNum = m_in.systemConfig.getSequenceNumber();
    if ( sysSeqNum != m_sysCfgSequenceNumber )
    {
        m_sysCfgSequenceNumber = sysSeqNum;
        m_equipment.reset();
    }

    // Call Equipment/Control logic (when there is a match in operating mode)
    if ( args.systemConfig.currentOpMode == m_equipment.getOperatingMode() )
    {
        // Execute the control logic 
        if ( m_equipment.executeActive( args ) == false )
        {
            // Put the outputs into a "all off state" if there is failure 
            m_out.vOutputs.setSafeAllOff();
            return false;
        }

        // Set my outputs
        m_out.vOutputs.write( args.vOutputs );
        m_out.cycleInfo.write( args.cycleInfo );
        m_out.systemOn.write( args.systemOn );
    }

    // Non-match operating mode
    else
    {
        return m_equipment.executeOff( args );
    }

    // If I get here -->everything worked!
    return true;
}


