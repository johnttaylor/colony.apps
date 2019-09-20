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


#include "SystemInfo.h"
#include "Cpl/System/Trace.h"


#define SECT_               "Storm::Component::SystemInfo"


/// Namespaces
using namespace Storm::Component;


///////////////////////////////
SystemInfo::SystemInfo( struct Input_T inputs, struct Output_T outputs )
    : m_in( inputs )
    , m_out( outputs )
{
}

bool SystemInfo::start( Cpl::System::ElapsedTime::Precision_T intervalTime )
{
    // Initialize parent class
    return Base::start( intervalTime );
}


///////////////////////////////
bool SystemInfo::execute( Cpl::System::ElapsedTime::Precision_T currentTick,
                          Cpl::System::ElapsedTime::Precision_T currentInterval
)
{
    CPL_SYSTEM_TRACE_FUNC( SECT_ );

    //--------------------------------------------------------------------------
    // Pre-Algorithm processing
    //--------------------------------------------------------------------------

    // Read my inputs
    Storm::Dm::MpIduConfig::Data iduConfig;
    Storm::Dm::MpOduConfig::Data oduConfig;
    int8_t                       validIduConfig    = m_in.iduConfig.read( iduConfig );
    int8_t                       validOduConfig    = m_in.oduConfig.read( oduConfig );
    if ( Cpl::Dm::ModelPoint::IS_VALID( validIduConfig ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validOduConfig ) == false )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "SystemInfo::execute. One or more invalid MPs (idcfg=%d, odcfg=%d)", validIduConfig, validOduConfig ) );

        // Invalid my output (and do NOTHING) if I don't have valid inputs...
        m_out.allowedModes.setInvalid();
        return true;
    }

    //--------------------------------------------------------------------------
    // Algorithm processing
    //--------------------------------------------------------------------------

    // Default to no active conditioning
    bool hasCooling  = false;
    bool hasHeating  = false;
    bool activeAlarm = true;

    // Is there heating capacity?
    if ( iduConfig.numHeatingStages > 0 )
    {
        hasHeating  = true;
        activeAlarm = false;
    }
    if ( oduConfig.numStages > 0 )
    {
        hasCooling  = true;
        activeAlarm = false;
    }

    //--------------------------------------------------------------------------
    // Post-Algorithm processing
    //--------------------------------------------------------------------------

    // Set allowed operating modes
    if ( hasCooling && hasHeating )
    {
        m_out.allowedModes.write( Storm::Type::AllowedOperatingModes::eCOOLING_AND_HEATING );
    }
    else if ( hasCooling )
    {
        m_out.allowedModes.write( Storm::Type::AllowedOperatingModes::eCOOLING_ONLY );
    }
    else if ( hasHeating )
    {
        m_out.allowedModes.write( Storm::Type::AllowedOperatingModes::eHEATING_ONLY );
    }
    else
    {
        m_out.allowedModes.setInvalid();
    }

    // Set alarm state
    if ( activeAlarm  )
    { 
        m_out.noActiveConditioningAlarm.setAlarm( true, true );
    }
    else
    {
        m_out.noActiveConditioningAlarm.setAlarm( false );
    }

    return true;
}




