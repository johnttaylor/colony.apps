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

#include "Storm/Thermostat/Main/Main.h"
#include "Storm/Thermostat/Main/Private_.h"
#include "Storm/Thermostat/_file_logger/Log.h"
#include "Storm/Thermostat/SimHouse/Cmd.h"
#include "Storm/Thermostat/SimHouse/House.h"
#include "Cpl/System/Thread.h"
#include <stdlib.h>

static Storm::Thermostat::SimHouse::Cmd     houseCmd_( g_cmdlist, "invoke_special_static_constructor" );
static Storm::Thermostat::Log               logCmd_( g_cmdlist, "invoke_special_static_constructor" );

static Storm::Thermostat::SimHouse::House   houseSimulator_;


////////////////////////////////////////////////////////////////////////////////
    
void initializePlatform0()
{
    // Create thread to run the House simulation
    Cpl::System::Thread::create( houseSimulator_, "HouseSim", CPL_SYSTEM_THREAD_PRIORITY_NORMAL );
}

void openPlatform0()
{
}

void closePlatform0()
{
}

int exitPlatform( int exitCode )
{
    exit( exitCode );
    return exitCode;
}

