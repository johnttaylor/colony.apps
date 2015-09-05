#ifndef Storm_Type_Rte_h_
#define Storm_Type_Rte_h_
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

#include "colony_config.h"
#include "Rte/Element/DataType.h"


/** The base offset for the RTE DataType magic numbers for all specialized
    datatypes defined in by the STORM:TYPE namespace (note this is GLOBAL with
    respect to Storm defined RTE DataTypes - NOT just this file). Only 
    override this value if there is collision with other Applicaiton defined 
    RTE DataType magic numbers.
 */
#ifndef OPTION_STORM_TYPE_RTE_DATATYPE_OFFSET
#define OPTION_STORM_TYPE_RTE_DATATYPE_OFFSET  100
#endif


/// Namespaces
namespace Storm { namespace Type { 

/** This class provide various Type definitions, enumerations, etc. used
    by the Storm namespace.
 */
class Rte
{
public: 
    /// RTE DataType magic number for TMode_T
    static const int DATATYPE_TMODE_T        = 0 + Rte::Element::DataType::START_APP_DEFINED + OPTION_STORM_TYPE_RTE_DATATYPE_OFFSET;

    /// RTE DataType magic number for DehumOption_T
    static const int DATATYPE_DEHUM_OPTION_T = 1 + Rte::Element::DataType::START_APP_DEFINED + OPTION_STORM_TYPE_RTE_DATATYPE_OFFSET;

    

};


};      // end namespace
#endif  // end header latch
