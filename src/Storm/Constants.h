#ifndef Storm_Constants_h_
#define Storm_Constants_h_
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
/** @file
    
    This file provides magic constants that NOT component specific, i.e. general
    system constants
 */


#include "cpl_config.h"

/** The maximum number of cooling stages that the system supports.  This is
    typically the same value as OPTION_STORM_DM_ODU_CONFIG_MAX_COMPRESSOR_STAGES
 */
#ifndef OPTION_STORM_MAX_COOLING_STAGES
#define OPTION_STORM_MAX_COOLING_STAGES        1
#endif

/** The maximum number of heating stages that the system supports.  This is
    typically the combination of the number of compressor heating stages and
    indoor heat stages 
  */
#ifndef OPTION_STORM_MAX_HEATING_STAGES
#define OPTION_STORM_MAX_HEATING_STAGES        1
#endif


#endif  // end header latch
