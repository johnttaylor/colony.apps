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
    Cpl::System::ElapsedTime::Precision_T beginOffTime      = { 0, 0 };
    Storm::Type::ThermostatMode           userMode          = Storm::Type::ThermostatMode::eOFF;
    int8_t                                validIdt          = m_in.idt.read( idt );
    int8_t                                validUserMode     = m_in.userMode.read( userMode );
    int8_t                                validSetpoints    = m_in.setpoints.read( coolSetpt, heatSetpt );
    int8_t                                validSystemOn     = m_in.systemOn.read( systemOn );
    int8_t                                validBeginOffTime = m_in.beginOffTime.read( beginOffTime );
    if ( Cpl::Dm::ModelPoint::IS_VALID( validIdt ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validUserMode ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validSetpoints ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validSystemOn ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( validBeginOffTime ) == false )
    {
        badInputs = true;
        CPL_SYSTEM_TRACE_MSG( SECT_, ("OperatingMode::execute. One or more invalid MPs (idt=%d, userMode=%d, setpts=%d, sysOn=%d, beginOff=%d", validIdt, validUserMode, validSetpoints, validSystemOn, validBeginOffTime) );
    }

    // Default the output value(s)
    m_out.operatingModeChanged.write( false );


    //--------------------------------------------------------------------------
    // Algorithm processing
    //--------------------------------------------------------------------------

    // Force the system off if/when my inputs are bad
    if ( badInputs )
    {
        setNewOperatingMode( Storm::Type::OperatingMode::eOFF );
    }

    // My Inputs are valid....
    else
    {
        // Convert the User Thermostat mode to Operating mode
        switch ( userMode )
        {
            // COOLING
        case Storm::Type::ThermostatMode::eCOOLING:
            m_inAuto = false;
            setNewOperatingMode( Storm::Type::OperatingMode::eCOOLING );
            break;

            // HEATING
        case Storm::Type::ThermostatMode::eHEATING:
            m_inAuto = false;
            setNewOperatingMode( Storm::Type::OperatingMode::eHEATING );
            break;

            // Resolve AUTO mode
        case Storm::Type::ThermostatMode::eAUTO:
            // Trap first time through
            if ( !m_inAuto )
            {
                m_inAuto = true;
                if ( idt <= heatSetpt )
                {
                    setNewOperatingMode( Storm::Type::OperatingMode::eHEATING );
                }
                else
                {
                    setNewOperatingMode( Storm::Type::OperatingMode::eCOOLING );
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
                        setNewOperatingMode( Storm::Type::OperatingMode::eCOOLING );
                    }
                    else
                    {
                        setNewOperatingMode( Storm::Type::OperatingMode::eHEATING );
                    }
                }
            }
            break;


            // OFF mode (and any invalid mode settings)
        default:
            m_inAuto = false;
            setNewOperatingMode( Storm::Type::OperatingMode::eOFF );
            break;
        }
    }

    // If I get here -->everything worked!
    return true;
}

/////////////////
void Storm::Component::OperatingMode::setNewOperatingMode( Storm::Type::OperatingMode newOpMode )
{
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

