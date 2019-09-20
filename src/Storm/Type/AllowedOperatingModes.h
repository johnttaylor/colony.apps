#ifndef Storm_Type_AllowedOperatingModes_h_
#define Storm_Type_AllowedOperatingModes_h_
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


#include "Cpl/Type/enum.h"


///
namespace Storm {
///
namespace Type {

/** The following enum/class defines allowed modes (as determine by the configured
    equipment/settings) that the system can operating in, e.g. furnace only 
    system can only operating in heating mode..

    \param AllowedOperatingModes    - Class name for the AllowedOperatingModes enum
    \param int                      - storage type of the enum
    \param eCOOLING_ONLY            - The can only operating in the cooling mode
    \param eHEATING_ONLY            - The can only operating in the heating mode
    \param eCOOLING_AND_HEATING     - The system can operating in both cooling and heating modes
 */
BETTER_ENUM( AllowedOperatingModes, int, eCOOLING_ONLY, eHEATING_ONLY, eCOOLING_AND_HEATING );


};      // end namespaces
};
#endif  // end header latch
