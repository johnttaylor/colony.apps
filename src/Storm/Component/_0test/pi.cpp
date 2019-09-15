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
#include "Storm/Component/Pi.h"
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Math/real.h"
#include "statics.h"


using namespace Storm::Component;

#define SECT_       "_0test"


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "PI" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();
    Pi::Input_T  ins  = { mp_resetPiPulse, mp_deltaIdtError, mp_piConstants, mp_freezePiRefCnt, mp_inhibitfRefCnt };
    Pi::Output_T outs = { mp_pvOut, mp_sumError, mp_pvInhibited };


    Pi component( ins, outs );

    // Start the component (and 'prime' it for the first real interval)
    Cpl::System::ElapsedTime::Precision_T time = { 0, 1 };
    component.start( time );
    component.doWork( true, time );
    time.m_thousandths += 1;

    
    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}

