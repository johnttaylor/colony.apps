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

#include "Storm/Thermostat/Main/Private_.h"
#include "Storm/Thermostat/Logger.h"
#include "task.h"



////////////////////////////////////////////////////////////////////////////////
void Storm::Thermostat::Logger::recordSystemData()
{
    // Not supported, at least not yet
}

void initializePlatform0()
{
}

void openPlatform0()
{
}

void closePlatform0()
{
}

int exitPlatform( int exitCode )
{
    vTaskEndScheduler();
    return exitCode;
}

