#ifndef Storm_Type_ComfortConfig_h_
#define Storm_Type_ComfortConfig_h_
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

#include "Storm/Constants.h"
#include "Storm/Type/Cph.h"


///
namespace Storm {
///
namespace Type {

/** The 'comfort' parameters per stage
 */
typedef struct
{
    int         cph;         //!< Cycle-Per-Hour settings.   The actual type is: Storm::Type::Cph ('betterenums' do not play well with classes/struct)
    uint32_t    minOnTime;   //!< Minimum on time in seconds
    uint32_t    minOffTime;  //!< Minimum off time in seconds
} ComfortStageParameters_T;

/** The 'comfort' parameters for the system
 */
typedef struct
{
    ComfortStageParameters_T cooling[OPTION_STORM_MAX_COMPRESSOR_COOLING_STAGES];   //!< Parameters for each cooling stages.  Cooling stages are zero-index, i.e. cooling stage1 is index 0
    ComfortStageParameters_T heating[OPTION_STORM_MAX_HEATING_STAGES];              //!< Parameters for each heating stages.  Heating stages are zero-index, i.e. cooling stage1 is index 0.  Compressor heating stages (if any) are ordered first (i.e. lower index) than the indoor heat stages.  For example a 2 stage HP with a 1 stage furnace: index 0 = 1st stage compressor, 1=2nd compressor stage, 2= 1st furnace stage
} ComfortConfig_T;



};      // end namespaces
};
#endif  // end header latch
