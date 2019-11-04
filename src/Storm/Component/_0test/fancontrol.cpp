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
#include "Storm/Component/FanControl.h"
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Math/real.h"
#include "statics.h"


using namespace Storm::Component;

#define SECT_       "_0test"


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "FanControl" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();
    FanControl::Input_T  ins  = { mp_fanMode,  mp_iduConfig, mp_comfortConfig, mp_systemType, mp_vOutputs, mp_equipmentBeingTimes, mp_systemForcedOffRefCnt };
    FanControl::Output_T outs = { mp_vOutputs };
    mp_iduConfig.writeFanMotor( true );
    mp_systemForcedOffRefCnt.reset();
    mp_fanMode.write( Storm::Type::FanMode::eAUTO );
    mp_systemType.write( Storm::Type::SystemType::eAC1_FURN1 );

    FanControl uut( ins, outs );

    // Start the components
    Cpl::System::ElapsedTime::Precision_T time = { 0, 1 };
    uut.start( time );

    // Fan Auto/ Vspeed Blower / !ForcedOff
    time.m_thousandths += 1;
    uut.doWork( true, time );
    Storm::Type::VirtualOutputs_T  outputValues;
    mp_vOutputs.read( outputValues );
    REQUIRE( outputValues.indoorFan == STORM_DM_MP_VIRTUAL_OUTPUTS_OFF );

    // Fan Continuous / Vspeed Blower / !ForcedOff
    mp_fanMode.write( Storm::Type::FanMode::eCONTINUOUS );
    time.m_thousandths += 1;
    uut.doWork( true, time );
    mp_vOutputs.read( outputValues );
    REQUIRE( outputValues.indoorFan == OPTION_STORM_COMPONENT_FAN_CONTROL_CONTINUOUS_SPEED );

    // Fan Continuous / Fixed speed Blower / !ForcedOff
    mp_iduConfig.writeFanMotor( false );
    time.m_thousandths += 1;
    uut.doWork( true, time );
    mp_vOutputs.read( outputValues );
    REQUIRE( outputValues.indoorFan == STORM_DM_MP_VIRTUAL_OUTPUTS_ON );

    // Fan Continuous / Fixed speed Blower / ForcedOff
    mp_systemForcedOffRefCnt.increment();
    time.m_thousandths += 1;
    uut.doWork( true, time );
    mp_vOutputs.read( outputValues );
    REQUIRE( outputValues.indoorFan == STORM_DM_MP_VIRTUAL_OUTPUTS_OFF );

    // Fan Continuous / Fixed speed Blower / !ForcedOff
    mp_systemForcedOffRefCnt.reset();
    time.m_thousandths += 1;
    uut.doWork( true, time );
    mp_vOutputs.read( outputValues );
    REQUIRE( outputValues.indoorFan == STORM_DM_MP_VIRTUAL_OUTPUTS_ON );

    // Fan Auto / Fixed speed Blower / !ForcedOff
    mp_vOutputs.setIndoorFanOutput( STORM_DM_MP_VIRTUAL_OUTPUTS_OFF );
    mp_fanMode.write( Storm::Type::FanMode::eAUTO );
    time.m_thousandths += 1;
    uut.doWork( true, time );
    mp_vOutputs.read( outputValues );
    REQUIRE( outputValues.indoorFan == STORM_DM_MP_VIRTUAL_OUTPUTS_OFF );

    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}

