/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2019  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "Catch/catch.hpp"
#include "Storm/Component/OperatingMode.h"
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Math/real.h"
#include "statics.h"


using namespace Storm::Component;

#define SECT_       "_0test"

////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "Operating Mode" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();
    OperatingMode::Input_T  ins  = { mp_setpoints,  mp_userMode, mp_activeIdt, mp_equipmentBeingTimes, mp_systemOn, mp_allowedOperatingModes, mp_systemForcedOffRefCnt, mp_systemType };
    OperatingMode::Output_T outs = { mp_operatingMode, mp_operatingModeChanged, mp_resetPiPulse, mp_operatingModeAlarm };
    OperatingMode           component( ins, outs );

    // Default values...
    mp_userMode.write( Storm::Type::ThermostatMode::eOFF );
    mp_setpoints.write( 78.0F, 68.0F );
    mp_activeIdt.write( 80.F );
    mp_systemOn.write( false );
    mp_freezePiRefCnt.reset();
    mp_resetPiPulse.write( false );
    mp_allowedOperatingModes.write( Storm::Type::AllowedOperatingModes::eCOOLING_AND_HEATING );
    mp_systemForcedOffRefCnt.reset();
    mp_systemType.write( Storm::Type::SystemType::eAC1_FURN1 );
    mp_operatingMode.write( Storm::Type::OperatingMode::eOFF );

    // Start the component (and 'prime' it for the first real interval)
    Cpl::System::ElapsedTime::Precision_T time = { 1, 0 };
    component.start( time );
    component.doWork( true, time );
    time.m_seconds += 1;

    SECTION( "cooling/heating/off mode" )
    {   
        mp_userMode.write( Storm::Type::ThermostatMode::eCOOLING );
        time.m_seconds += 1;
        component.doWork( true, time );

        Storm::Type::OperatingMode value = Storm::Type::OperatingMode::eOFF;
        int8_t valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eCOOLING );
        bool changed;
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == true );
        bool reset;
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == true );
        mp_resetPiPulse.write( false );

        time.m_seconds += 1;
        component.doWork( true, time );
        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eCOOLING );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == false );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == false );

        mp_userMode.write( Storm::Type::ThermostatMode::eHEATING );
        time.m_seconds += 1;
        component.doWork( true, time );

        mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eID_HEATING );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == true );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == true );
        mp_resetPiPulse.write( false );

        time.m_seconds += 1;
        component.doWork( true, time );
        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eID_HEATING );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == false );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == false );

        mp_userMode.write( Storm::Type::ThermostatMode::eOFF );
        time.m_seconds += 1;
        component.doWork( true, time );

        mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eOFF );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == true );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == true );
        mp_resetPiPulse.write( false );

        time.m_seconds += 1;
        component.doWork( true, time );
        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eOFF );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == false );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == false );
    }

    SECTION( "bad inputs mode" )
    {
        mp_activeIdt.setInvalid();
        mp_userMode.write( Storm::Type::ThermostatMode::eCOOLING );
        time.m_seconds += 1;
        component.doWork( true, time );

        Storm::Type::OperatingMode value = Storm::Type::OperatingMode::eOFF;
        int8_t valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eOFF );
        bool changed;
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == false );
        bool reset;
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == false );

        mp_activeIdt.write( 80.0F );
        time.m_seconds += 1;
        component.doWork( true, time );
        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eCOOLING );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == true );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == true );

        // Forced Off
        mp_systemForcedOffRefCnt.increment();
        mp_activeIdt.write( 80.0F );
        time.m_seconds += 1;
        component.doWork( true, time );
        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eOFF );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == true );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == true );

        // Forced released
        mp_systemForcedOffRefCnt.reset();
        mp_activeIdt.write( 80.0F );
        time.m_seconds += 1;
        component.doWork( true, time );
        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eCOOLING );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == true );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == true );
    }

    SECTION( "auto mode - initial transitions" )
    {
        // Initial transition to auto (expected to resolve to cooling mode)
        mp_setpoints.write( 78.0F, 68.0F );
        mp_activeIdt.write( 70.0F );
        mp_userMode.write( Storm::Type::ThermostatMode::eAUTO );
        time.m_seconds += 1;
        component.doWork( true, time );

        Storm::Type::OperatingMode value = Storm::Type::OperatingMode::eOFF;
        int8_t valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eCOOLING );
        bool changed;
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == true );
        bool reset;
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == true );
        mp_resetPiPulse.write( false );

        time.m_seconds += 1;
        component.doWork( true, time );
        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eCOOLING );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == false );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == false );

        // Turn off
        mp_userMode.write( Storm::Type::ThermostatMode::eOFF );
        time.m_seconds += 1;
        component.doWork( true, time );
        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eOFF );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == true );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == true );
        mp_resetPiPulse.write( false );

        // Initial transition to auto (expected to resolve to heating mode)
        mp_setpoints.write( 78.0F, 68.0F );
        mp_activeIdt.write( 68.0F );
        mp_userMode.write( Storm::Type::ThermostatMode::eAUTO );
        time.m_seconds += 1;
        component.doWork( true, time );

        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eID_HEATING );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == true );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == true );
        mp_resetPiPulse.write( false );

        time.m_seconds += 1;
        component.doWork( true, time );
        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eID_HEATING );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == false );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == false );
    }

    SECTION( "auto mode - changeover to cooling" )
    {
        // Initial transition to auto (expected to resolve to heating mode)
        mp_setpoints.write( 78.0F, 68.0F );
        mp_activeIdt.write( 67.0F );
        mp_userMode.write( Storm::Type::ThermostatMode::eAUTO );
        time.m_seconds += 1;
        component.doWork( true, time );

        Storm::Type::OperatingMode value = Storm::Type::OperatingMode::eOFF;
        int8_t valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eID_HEATING );
        bool changed;
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == true );
        bool reset;
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == true );
        mp_resetPiPulse.write( false );
        mp_equipmentBeingTimes.setSystemBeginOffTime( time );

        // Change the current temp to meet the criteria for switching to cooling (BUT not enough time has elapsed)
        mp_activeIdt.write( 78.0F );
        time.m_seconds += OPTION_STORM_COMPONENT_OPERATING_MODE_SECONDS_HYSTERESIS - 1;
        component.doWork( true, time );

        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eID_HEATING );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == false );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == false );

        // Elapsed enough time for the change
        time.m_seconds += 1;
        component.doWork( true, time );

        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eCOOLING );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == true );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == true );
        mp_resetPiPulse.write( false );
    }

    SECTION( "auto mode - changeover to heating" )
    {
        // Initial transition to auto (expected to resolve to cooling mode)
        mp_setpoints.write( 78.0F, 68.0F );
        mp_activeIdt.write( 72.0F );
        mp_userMode.write( Storm::Type::ThermostatMode::eAUTO );
        time.m_seconds += 1;
        component.doWork( true, time );

        Storm::Type::OperatingMode value = Storm::Type::OperatingMode::eOFF;
        int8_t valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eCOOLING );
        bool changed;
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == true );
        bool reset;
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == true );
        mp_resetPiPulse.write( false );
        mp_equipmentBeingTimes.setIndoorBeginOffTime( time );

        // Change the current temp to ALMOST meet the criteria for switching to heating (BUT not enough time has elapsed)
        mp_activeIdt.write( 78.0F - ( OPTION_STORM_COMPONENT_OPERATING_MODE_COOLING_OFFSET / 2.0F ) );
        time.m_seconds += OPTION_STORM_COMPONENT_OPERATING_MODE_SECONDS_HYSTERESIS - 1;
        component.doWork( true, time );

        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eCOOLING );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == false );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == false );

        // Elapsed enough time for the change (but still not heating criteria)
        time.m_seconds += 1;
        component.doWork( true, time );

        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eCOOLING );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == false );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == false );

        // Meet the heating criteria)
        mp_activeIdt.write( 78.0F - OPTION_STORM_COMPONENT_OPERATING_MODE_COOLING_OFFSET * 1.5F );
        time.m_seconds += 1;
        component.doWork( true, time );

        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eID_HEATING );
        valid = mp_operatingModeChanged.read( changed );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( changed == true );
        valid = mp_resetPiPulse.read( reset );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( reset == true );
    }

    SECTION( "restricted operating modes" )
    {
        // Only Heating
        mp_userMode.write( Storm::Type::ThermostatMode::eCOOLING );
        mp_allowedOperatingModes.write( Storm::Type::AllowedOperatingModes::eHEATING_ONLY );
        time.m_seconds += 1;
        component.doWork( true, time );

        Storm::Type::OperatingMode value = Storm::Type::OperatingMode::eCOOLING;
        int8_t valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eOFF );
        Storm::Dm::MpSimpleAlarm::Data alarm;
        mp_operatingModeAlarm.read( alarm );
        REQUIRE( alarm.active == true );
        REQUIRE( alarm.critical == true );

        mp_allowedOperatingModes.write( Storm::Type::AllowedOperatingModes::eCOOLING_AND_HEATING );
        time.m_seconds += 1;
        component.doWork( true, time );

        value = Storm::Type::OperatingMode::eCOOLING;
        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eCOOLING );
        mp_operatingModeAlarm.read( alarm );
        REQUIRE( alarm.active == false );
        REQUIRE( alarm.critical == false );

        // Only Cooling
        mp_userMode.write( Storm::Type::ThermostatMode::eHEATING );
        mp_allowedOperatingModes.write( Storm::Type::AllowedOperatingModes::eCOOLING_ONLY );
        time.m_seconds += 1;
        component.doWork( true, time );

        value = Storm::Type::OperatingMode::eCOOLING;
        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eOFF );
        mp_operatingModeAlarm.read( alarm );
        REQUIRE( alarm.active == true );
        REQUIRE( alarm.critical == true );

        mp_allowedOperatingModes.write( Storm::Type::AllowedOperatingModes::eCOOLING_AND_HEATING );
        time.m_seconds += 1;
        component.doWork( true, time );

        value = Storm::Type::OperatingMode::eCOOLING;
        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eID_HEATING );
        mp_operatingModeAlarm.read( alarm );
        REQUIRE( alarm.active == false );
        REQUIRE( alarm.critical == false );

        // Auto - Heating only
        mp_userMode.write( Storm::Type::ThermostatMode::eAUTO );
        mp_allowedOperatingModes.write( Storm::Type::AllowedOperatingModes::eHEATING_ONLY );
        time.m_seconds += 1;
        component.doWork( true, time );

        value = Storm::Type::OperatingMode::eCOOLING;
        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eID_HEATING );
        mp_operatingModeAlarm.read( alarm );
        REQUIRE( alarm.active == false );
        REQUIRE( alarm.critical == false );

        // Auto - Cooling only
        mp_userMode.write( Storm::Type::ThermostatMode::eAUTO );
        mp_allowedOperatingModes.write( Storm::Type::AllowedOperatingModes::eCOOLING_ONLY );
        time.m_seconds += 1;
        component.doWork( true, time );

        value = Storm::Type::OperatingMode::eCOOLING;
        valid = mp_operatingMode.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::OperatingMode::eCOOLING );
        mp_operatingModeAlarm.read( alarm );
        REQUIRE( alarm.active == false );
        REQUIRE( alarm.critical == false );
    }

    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}

