#ifndef Storm_Thermostat_Api_h_
#define Storm_Thermostat_Api_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Apps Project.  The Colony.Apps Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.apps/license.txt
*
* Copyright (c) 2015 John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

#include "Storm/Thermostat/DataDictionary.h"


/// Namespaces
namespace Storm { namespace Thermostat {


/** This class is the public (to other namespaces) for the Storm::Thermostat
    Applicaiton and/or Data Dictionary
 */
class Api
{
public:
    /** Returns the Model Point for the Thermostat's Data Dictionary
     */
    virtual DataDictionaryModel& getDDModel( void ) = 0;
};



};      // end namespace
};      
#endif  // end header latch
