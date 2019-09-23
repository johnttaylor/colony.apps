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


#include "OperatingMode.h"
#include "Cpl/System/Trace.h"


#define SECT_                   "Storm::Component::OperatingMode"


/// Namespaces
using namespace Storm::Component;


///////////////////////////////
OperatingMode::OperatingMode( struct Input_T ins, struct Output_T outs )
    : m_in( ins )
    , m_out( outs )
    , m_prevOperatingMode( Storm::Type::ThermostatMode::eOFF )
    , m_inAuto( false )
{
}

bool OperatingMode::start( Cpl::System::ElapsedTime::Precision_T intervalTime )
{
    // Initialize my data
    m_prevOperatingMode  = Storm::Type::ThermostatMode::eOFF;
    m_inAuto             = false;

    // Initialize parent class
    return Base::start( intervalTime );
}


///////////////////////////////
bool OperatingMode::execute( Cpl::System::ElapsedTime::Precision_T currentTick,
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
    float                                 heatSetpt         = 0.0F;
    float                                 coolSetpt         = 0.0F;
    bool                                  systemOn          = false;
    uint32_t                              forceOffRefCnt    = 0;
    Cpl::System::ElapsedTime::Precision_T beginOffTime      = { 0, 0 };
    Storm::Type::ThermostatMode           userMode          = Storm::Type::ThermostatMode::eOFF;
    Storm::Type::AllowedOperatingModes    allowedModes      = Storm::Type::AllowedOperatingModes::eCOOLING_AND_HEATING;
    Storm::Type::SystemType               systemType        = Storm::Type::SystemType::eUNDEFINED;
    int8_t                                validIdt          = m_in.idt.read( idt );
    int8_t                                validUserMode     = m_in.userMode.read( userMode );
    int8_t                                validSetpoints    = m_in.setpoints.read( coolSetpt, heatSetpt );
    int8_t                                validSystemOn     = m_in.systemOn.read( systemOn );
    int8_t                                validBeginOffTime = m_in.beginOffTime.read( beginOffTime );
    int8_t                                validAllowedModes = m_in.allowedModes.read( allowedModes );
    int8_t                                validForceOff     = m_in.systemForcedOffRefCnt.read( forceOffRefCnt );
    int8_t                                validSystemType   = m_in.systemType.read( systemType );
    if ( Cpl::Dm::ModelPoint::IS_VALID( validIdt ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validUserMode ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validSetpoints ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validSystemOn ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validBeginOffTime ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validForceOff ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validSystemType ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validAllowedModes ) == false )
    {
        badInputs = true;
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "OperatingMode::execute. One or more invalid MPs (idt=%d, userMode=%d, setpts=%d, sysOn=%d, beginOff=%d, forcedOff=%d, allowedModes=%d, systemType=%d", validIdt, validUserMode, validSetpoints, validSystemOn, validBeginOffTime, validForceOff, validAllowedModes, validSystemType ) );
    }

    // Default the output value(s)
    m_out.operatingModeChanged.write( false );


    //--------------------------------------------------------------------------
    // Algorithm processing
    //--------------------------------------------------------------------------

    // Force the system off if/when my inputs are bad OR I have been 'forced' off
    if ( badInputs || forceOffRefCnt > 0 )
    {
        setNewOperatingMode( Storm::Type::OperatingMode::eOFF, systemType );
    }

    // My Inputs are valid....
    else
    {
        bool alarmActive = false;

        // 'correct' usermode based on what the system is capable of
        if ( userMode == +Storm::Type::ThermostatMode::eCOOLING && allowedModes == +Storm::Type::AllowedOperatingModes::eHEATING_ONLY )
        {
            userMode    = Storm::Type::ThermostatMode::eOFF;
            alarmActive = true;
        }
        else if ( (userMode == +Storm::Type::ThermostatMode::eHEATING || userMode == +Storm::Type::ThermostatMode::eID_HEATING) && allowedModes == +Storm::Type::AllowedOperatingModes::eCOOLING_ONLY )
        {
            userMode    = Storm::Type::ThermostatMode::eOFF;
            alarmActive = true;
        }
        else if ( userMode == +Storm::Type::ThermostatMode::eAUTO && allowedModes != +Storm::Type::AllowedOperatingModes::eCOOLING_AND_HEATING )
        {
            if ( allowedModes == +Storm::Type::AllowedOperatingModes::eCOOLING_ONLY )
            {
                userMode = Storm::Type::ThermostatMode::eCOOLING;
            }
            else
            {
                userMode = Storm::Type::ThermostatMode::eHEATING;
            }
        }

        // Throw an alarm if the system has been forced off due to the 'bad' user/system modes
        if ( alarmActive )
        {
            m_out.userConfigModeAlarm.setAlarm( true, true );
        }
        else
        {
            m_out.userConfigModeAlarm.setAlarm( false );
        }

        // Convert the User Thermostat mode to Operating mode
        switch ( userMode )
        {
            // COOLING
        case Storm::Type::ThermostatMode::eCOOLING:
            m_inAuto = false;
            setNewOperatingMode( Storm::Type::OperatingMode::eCOOLING, systemType );
            break;

            // HEATING
        case Storm::Type::ThermostatMode::eHEATING:
            m_inAuto = false;
            setNewOperatingMode( Storm::Type::OperatingMode::eHEATING, systemType );
            break;

        case Storm::Type::ThermostatMode::eID_HEATING:
            m_inAuto = false;
            setNewOperatingMode( Storm::Type::OperatingMode::eID_HEATING, systemType );
            break;

            // Resolve AUTO mode
        case Storm::Type::ThermostatMode::eAUTO:
            // Trap first time through
            if ( !m_inAuto )
            {
                m_inAuto = true;
                if ( idt <= heatSetpt )
                {
                    setNewOperatingMode( Storm::Type::OperatingMode::eHEATING, systemType );
                }
                else
                {
                    setNewOperatingMode( Storm::Type::OperatingMode::eCOOLING, systemType );
                }
            }

            // Nominal path
            else
            {
                static const Cpl::System::ElapsedTime::Precision_T timeHysteresis = { OPTION_STORM_COMPONENT_OPERATING_MODE_SECONDS_HYSTERESIS, 0 };

                // Only switch modes if the system has been off for at least N seconds.
                if ( systemOn == false && Cpl::System::ElapsedTime::expiredPrecision( beginOffTime, timeHysteresis, currentInterval ) )
                {
                    if ( idt >= coolSetpt - OPTION_STORM_COMPONENT_OPERATING_MODE_COOLING_OFFSET )
                    {
                        setNewOperatingMode( Storm::Type::OperatingMode::eCOOLING, systemType );
                    }
                    else
                    {
                        setNewOperatingMode( Storm::Type::OperatingMode::eHEATING, systemType );
                    }
                }
            }
            break;


            // OFF mode (and any invalid mode settings)
        default:
            m_inAuto = false;
            setNewOperatingMode( Storm::Type::OperatingMode::eOFF, systemType );
            break;
        }
    }

    // If I get here -->everything worked!
    return true;
}

/////////////////
void Storm::Component::OperatingMode::setNewOperatingMode( Storm::Type::OperatingMode newOpMode, Storm::Type::SystemType systemType )
{
    // Trap the case of 'Heating' but ONLY indoor heat is available
    if ( newOpMode == +Storm::Type::OperatingMode::eHEATING && ( ( ( uint16_t) systemType ) & STORM_TYPE_SYSTEM_TYPE_ODUNIT_MASK ) != STORM_TYPE_SYSTEM_TYPE_HP_BITS )
    {
        newOpMode = Storm::Type::OperatingMode::eID_HEATING;
    }

    // React to a change in the operating mode
    if ( newOpMode != m_prevOperatingMode )
    {
        // Set indication that the Operating mode is/was changed
        m_out.operatingModeChanged.write( true );

        // Reset PI on mode changes
        m_out.pulseResetPi.write( true );

        // Set the new mode and cache it       
        m_prevOperatingMode = newOpMode;
        m_out.operatingMode.write( newOpMode );
    }
}

