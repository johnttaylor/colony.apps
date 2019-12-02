#ifndef Storm_Thermostat_0test_Log_h
#define Storm_Thermostat_0test_Log_h
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
/** @file */

#include "colony_config.h"
#include "Cpl/TShell/Cmd/Command.h"

/** Command
                                                "         1         2         3         4         5         6         7         8"
                                                "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
*/
#define STORMTHERMOSTAT0TESTCMD_CMD_LOG_	    "log"
/// Usage
#define STORMTHERMOSTAT0TESTCMD_USAGE_LOG_      "log\n" \
                                                "log enable <fname>\n" \
                                                "log disable"

/// Detailed Help text
#ifndef STORMTHERMOSTAT0TESTCMD_DETAIL_LOG_    
#define STORMTHERMOSTAT0TESTCMD_DETAIL_LOG_   "  Enables/disables logging to a CSV file."

#endif // ifndef allows detailed help to be compacted down to a single character if FLASH/code space is an issue



/** This class implements a  Shell command.
 */
class Log : public Cpl::TShell::Cmd::Command
{
protected:
    /// See Cpl::TShell::Command
    const char* getUsage() const noexcept { return STORMTHERMOSTAT0TESTCMD_USAGE_LOG_; }

    /// See Cpl::TShell::Command
    const char* getHelp() const noexcept { return STORMTHERMOSTAT0TESTCMD_DETAIL_LOG_; }


public:
    /// Constructor
    Log( Cpl::Container::Map<Cpl::TShell::Command>& commandList ) noexcept;

    /// Constructor.  Used to create a static instance of the command
    Log( Cpl::Container::Map<Cpl::TShell::Command>& commandList, const char* ignoreThisParameter_onlyUsedWhenCreatingAStaticInstance ) noexcept;

public:
    /// See Cpl::TShell::Command
    Cpl::TShell::Command::Result_T execute( Cpl::TShell::Context_& context, char* cmdString, Cpl::Io::Output& outfd ) noexcept;

};

#endif  // end header latch
