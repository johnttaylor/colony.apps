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
    , m_forcedOff( false )
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
    bool                         badInputs = false;
    Storm::Dm::MpIduConfig::Data iduConfig;
    Storm::Dm::MpOduConfig::Data oduConfig;
    int8_t                       validIduConfig    = m_in.iduConfig.read( iduConfig );
    int8_t                       validOduConfig    = m_in.oduConfig.read( oduConfig );
    if ( Cpl::Dm::ModelPoint::IS_VALID( validIduConfig ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validOduConfig ) == false )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "SystemInfo::execute. One or more invalid MPs (idcfg=%d, odcfg=%d)", validIduConfig, validOduConfig ) );

        badInputs = true;
    }

    //--------------------------------------------------------------------------
    // Algorithm processing
    //--------------------------------------------------------------------------

    // Default the System Type to: unknown
    uint16_t systemType  = Storm::Type::SystemType::eUNDEFINED;
    bool     hasCooling  = false;
    bool     hasHeating  = false;

    // Skip further processing if I don't have all of my inputs
    if ( badInputs == false )
    {
        // Set Outdoor unit type (Note: The ODUNIT check MUST be done FIRST)
        if ( oduConfig.type == Storm::Type::OduType::eAC )
        {
            systemType = STORM_TYPE_SYSTEM_TYPE_AC_BITS;
            if ( oduConfig.numStages > 0 )
            {
                hasCooling  = true;
            }
            else
            {
                systemType = STORM_TYPE_SYSTEM_TYPE_NO_ODUNIT_BITS;
            }
        }
        else if ( oduConfig.type == Storm::Type::OduType::eHP )
        {
            systemType = STORM_TYPE_SYSTEM_TYPE_HP_BITS;
            if ( oduConfig.numStages > 0 )
            {
                hasCooling  = true;
                hasHeating  = true;
            }
            else
            {
                systemType = STORM_TYPE_SYSTEM_TYPE_NO_ODUNIT_BITS;
            }
        }
        else
        {
            systemType |= STORM_TYPE_SYSTEM_TYPE_NO_ODUNIT_BITS;
        }

        // Set Indoor unit type
        if ( iduConfig.type == Storm::Type::IduType::eAIR_HANDLER )
        {
            systemType |= STORM_TYPE_SYSTEM_TYPE_AH_BITS;
        }
        else if ( iduConfig.type == Storm::Type::IduType::eFURNACE )
        {
            systemType |= STORM_TYPE_SYSTEM_TYPE_FURN_BITS;
        }

        // Set number of outdoor/indoor stages
        systemType |= oduConfig.numStages << STORM_TYPE_SYSTEM_TYPE_OD_STAGE_SHIFT_BITS;
        systemType |= iduConfig.numHeatingStages << STORM_TYPE_SYSTEM_TYPE_ID_STAGE_SHIFT_BITS;
        if ( iduConfig.numHeatingStages > 0 )
        {
            hasHeating = true;
        }

        // Is the system configuration a valid/supported configuration?
        auto maybe = Storm::Type::SystemType::_from_integral_nothrow( systemType );
        if ( !maybe )
        {
            systemType = Storm::Type::SystemType::eUNDEFINED;
        }
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
    if ( systemType == +Storm::Type::SystemType::eUNDEFINED )
    {
        m_out.noActiveConditioningAlarm.setAlarm( true, true );
        if ( m_forcedOff == false )
        {
            m_out.systemForcedOffRefCnt.increment();
            m_forcedOff = true;
        }
    }
    else
    {
        m_out.noActiveConditioningAlarm.setAlarm( false );
        if ( m_forcedOff == true )
        {
            m_out.systemForcedOffRefCnt.decrement();
            m_forcedOff = false;
        }
    }

    // Set system type
    m_out.systemType.write( Storm::Type::SystemType::_from_integral_unchecked( systemType ) );
    return true;
}




