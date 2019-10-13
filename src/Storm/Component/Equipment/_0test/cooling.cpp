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
#include "Storm/Component/Equipment/Cooling.h"
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Math/real.h"
#include "MyStage.h"


using namespace Storm::Component::Equipment;

#define SECT_       "_0test"



////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "Cooling" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();

    MyStage stage1;
    Cooling uut(stage1);
    Storm::Component::Control::Equipment::Args_T args = { 0, };
    args.vOutputs.sovInHeating                        = true;
    args.currentInterval                              = { 1, 0 };
    args.currentTick                                  = { 1, 1 };
    args.systemOn                                     = false;
    args.pvOut                                        = OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_ON_THRESHOLD + 1.0F;
    args.comfortConfig.cooling[0].minOffTime          = 60;
    args.comfortConfig.cooling[0].minOnTime           = 90;

    // start() and reset()
    REQUIRE( stage1.m_countReset == 0 );
    uut.start( { 0, 100 } );
    REQUIRE( stage1.m_countReset == 1 );
    uut.reset();
    REQUIRE( stage1.m_countReset == 2 );
    stage1.m_countReset = 0;

    // request to off
    REQUIRE( stage1.m_countRequestModeToOff == 0 );
    uut.executeOff( Storm::Type::SystemType::eAC1_FURN1, args );
    REQUIRE( stage1.m_countRequestModeToOff == 1 );

    // Transition from system off to system on
    REQUIRE( stage1.m_countExecute == 0 );
    args.currentInterval += { OPTION_STORM_MIN_COMPRESSOR_OFF_TIME_SEC - 10, 0 };
    uut.executeActive( Storm::Type::SystemType::eAC1_FURN1, args );
    REQUIRE( stage1.m_countExecute == 1 );
    REQUIRE( stage1.m_countRequestOn == 0 );
    args.currentInterval += { OPTION_STORM_MIN_COMPRESSOR_OFF_TIME_SEC + 1, 0 };
    uut.executeActive( Storm::Type::SystemType::eAC1_FURN1, args );
    REQUIRE( stage1.m_countExecute == 2 );
    REQUIRE( stage1.m_countRequestOn == 1 );
    args.cycleInfo.beginOnTime = args.currentInterval;
    stage1.m_isOnCycle         = true;
    stage1.m_isOffCycle        = false;
    stage1.m_isActive          = true;
    args.systemOn              = true;

    // Running - no change in stage(s)
    args.currentInterval += { 1, 0 }; 
    uut.executeActive( Storm::Type::SystemType::eAC1_FURN1, args );
    REQUIRE( stage1.m_countExecute == 3 );
    REQUIRE( stage1.m_countRequestOn == 1 );

    // Transition from Running to off
    args.pvOut            = OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_OFF_THRESHOLD;
    args.currentInterval += { 1, 0 };
    uut.executeActive( Storm::Type::SystemType::eAC1_FURN1, args );
    REQUIRE( stage1.m_countExecute == 4 );
    REQUIRE( stage1.m_countRequestOn == 1 );
    REQUIRE( stage1.m_countRequestOff == 0 );
    args.currentInterval += { args.comfortConfig.cooling[0].minOnTime, 0 };
    uut.executeActive( Storm::Type::SystemType::eAC1_FURN1, args );
    REQUIRE( stage1.m_countExecute == 5 );
    REQUIRE( stage1.m_countRequestOn == 1 );
    REQUIRE( stage1.m_countRequestOff == 1 );
    REQUIRE( stage1.m_countRequestOffWithLowerStage == 0 );
    stage1.m_isOnCycle  = false;
    stage1.m_isOffCycle = true;
    args.systemOn       = false;

    // Transition from system off to system on
    args.pvOut = OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_ON_THRESHOLD + 1.0F;
    args.currentInterval += { OPTION_STORM_MIN_COMPRESSOR_OFF_TIME_SEC + 1, 0 };
    uut.executeActive( Storm::Type::SystemType::eAC1_FURN1, args );
    REQUIRE( stage1.m_countExecute == 6 );
    REQUIRE( stage1.m_countRequestOn == 2 );
    args.equipmentBeginTimes.outdoorUnitBeginOnTime = args.currentInterval;
    args.cycleInfo.beginOnTime = args.currentInterval;
    stage1.m_isOnCycle         = true;
    stage1.m_isOffCycle        = false;
    stage1.m_isActive          = true;
    args.systemOn              = true;


    // Transition from Running to off (while in off cycle)
    stage1.m_isOnCycle    = false;
    stage1.m_isOffCycle   = true;
    args.pvOut            = OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_OFF_THRESHOLD - 1.0F;
    args.currentInterval += { 1, 0 };
    uut.executeActive( Storm::Type::SystemType::eAC1_FURN1, args );
    REQUIRE( stage1.m_countExecute == 7 );
    REQUIRE( stage1.m_countRequestOn == 2 );
    REQUIRE( stage1.m_countRequestOff == 2 );
    REQUIRE( stage1.m_countRequestOffWithLowerStage == 0 );
    stage1.m_isOnCycle = false;
    args.systemOn      = false;

    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}

