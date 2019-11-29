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
#include "Storm/Thermostat/Algorithm.h"
#include "Storm/Thermostat/ModelPoints.h"
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Math/real.h"


using namespace Storm::Thermostat;

#define SECT_       "_0test"


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "Algorithm" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();

    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}

