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
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Storm/Component/Equipment/Stage/BasicCooling.h"


using namespace Storm::Component::Equipment::Stage;


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "Basic Cooling" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();
    Storm::Component::Control::Equipment::Args_T args = { 0, };
    BasicCooling uut;
    args.comfortConfig.cooling[0].cph        = Storm::Type::Cph::e3CPH;
    args.comfortConfig.cooling[0].minOffTime = 300;
    args.comfortConfig.cooling[0].minOnTime  = 100;

    SECTION( "FSM transitions" )
    {
        // Reset/Init
        REQUIRE( uut.isInOff() );
        REQUIRE( uut.isActive() == false );

        // Start in an Off Cycle
        args.pvOut = 99.0F;
        uut.requestOn( args, false );
        REQUIRE( uut.isActive() );
        REQUIRE( uut.isInOffCycle() );
        REQUIRE( args.cycleInfo.mode == Storm::Type::CycleStatus::eOFF_CYCLE );
        REQUIRE( args.cycleInfo.offTime == args.comfortConfig.cooling[0].minOffTime );
        REQUIRE( args.vOutputs.indoorFan == STORM_DM_MP_VIRTUAL_OUTPUTS_OFF );
        REQUIRE( args.vOutputs.outdoorStages[0] == STORM_DM_MP_VIRTUAL_OUTPUTS_OFF );
        REQUIRE( args.vOutputs.sovInHeating == false );

        // No load
        uut.requestOff( args );
        REQUIRE( uut.isActive() == false );
        REQUIRE( uut.isOff() );

        // Start in an On Cycle
        args.pvOut = 1.0F;
        uut.requestOn( args, true );
        REQUIRE( uut.isActive() );
        REQUIRE( uut.isInOnCycle() );
        REQUIRE( args.cycleInfo.mode == Storm::Type::CycleStatus::eON_CYCLE );
        REQUIRE( args.cycleInfo.onTime == args.comfortConfig.cooling[0].minOnTime );
        REQUIRE( args.vOutputs.indoorFan == STORM_DM_MP_VIRTUAL_OUTPUTS_ON );
        REQUIRE( args.vOutputs.outdoorStages[0] == STORM_DM_MP_VIRTUAL_OUTPUTS_ON );
        REQUIRE( args.vOutputs.sovInHeating == false );

        // Transition from On to Off cycle
        args.currentInterval += { args.comfortConfig.cooling[0].minOnTime + 1, 0 };
        uut.execute( args );
        REQUIRE( uut.isActive() );
        REQUIRE( uut.isInOffCycle() );
        REQUIRE( args.cycleInfo.mode == Storm::Type::CycleStatus::eOFF_CYCLE );
        REQUIRE( args.vOutputs.indoorFan == STORM_DM_MP_VIRTUAL_OUTPUTS_OFF );
        REQUIRE( args.vOutputs.outdoorStages[0] == STORM_DM_MP_VIRTUAL_OUTPUTS_OFF );
        REQUIRE( args.vOutputs.sovInHeating == false );

        // Transition from Off to On cycle
        args.currentInterval += { args.cycleInfo.offTime+ 1, 0 };
        uut.execute( args );
        REQUIRE( uut.isActive() );
        REQUIRE( uut.isInOnCycle() );
        REQUIRE( args.cycleInfo.mode == Storm::Type::CycleStatus::eON_CYCLE );
        REQUIRE( args.vOutputs.indoorFan == STORM_DM_MP_VIRTUAL_OUTPUTS_ON );
        REQUIRE( args.vOutputs.outdoorStages[0] == STORM_DM_MP_VIRTUAL_OUTPUTS_ON );
        REQUIRE( args.vOutputs.sovInHeating == false );
    }


    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}

