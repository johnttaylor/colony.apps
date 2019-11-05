#ifndef Storm_Type_SystemConfig_h_
#define Storm_Type_SystemConfig_h_
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


#include "Storm/Type/OperatingMode.h"
#include "Storm/Type/IduType.h"
#include "Storm/Type/OduType.h"
#include "Storm/Constants.h"

///
namespace Storm {
///
namespace Type {

/** The following typedef information per stage
 */
typedef struct
{
    float       lowerBound;       //!< Inclusive lower bound for the PV variable (aka load)
    float       upperBound;       //!< Inclusive upper bound for the PV variable (aka load)
    uint16_t    minIndoorFan;     //!< Minimum indoor fan speed associated with the stage (Range: 0=off, 1000=Full speed)
    uint16_t    maxIndoorFan;     //!< Maximum indoor fan speed associated with the stage (Range: 0=off, 1000=Full speed)
} Stage_T;

/** The following typedef contains system attributes, parameters, bounds, etc. 
    as derived from the current equipment configuration.
 */
typedef struct
{
    Stage_T     stages[OPTION_STORM_MAX_TOTAL_STAGES];      //!< Configuration/Information by stage.  Zero indexed by stage (same indexing scheme as the ComfortConfig settings). For example: 2 stage compressor with 2 stage furnace would be: [0]=1st compressor stage, [1]=2nd compressor stage, [3]=1st furnace stage, [4]=2nd furnace stage 
    int         allowedOperatingModes;                      //!< Allowed modes of operation.  The actual type is: Storm::Type::OperatingMode ('betterenums' do not play well with classes/struct)
    int         outdoorUnitType;                            //!< Outdoor Unit type. The actual type is: Storm::Type::OduType
    int         indoorUnitType;                             //!< Indoor Unit Type. The actual type is: Storm::Type::IduType
    uint16_t    numCompressorStages;                        //!< Number of active/configured compressor stages
    uint16_t    numIndoorStages;                            //!< Number of  active/configured indoor stages
    uint16_t    totalStages;                                //!< Total number of active stages (i.e. numCompressorStages + numIndoorStages)
    uint16_t    fanContinuousSpeed;                         //!< Indoor Fan Continuous speed setting.  (Range: 0=off, 1000=Full speed)
} SystemConfig_T;



};      // end namespaces
};
#endif  // end header latch
