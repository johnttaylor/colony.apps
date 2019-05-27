#ifndef Storm_Type_Tuple_StageConfig_h_
#define Storm_Type_Tuple_StageConfig_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Apps Project.  The Colony.Apps Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.apps/license.txt
*
* Copyright (c) 2015 - 2019 John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

#include "Rte/Tuple/Basic.h"
#include "Rte/Element/Basic.h"




#define STORM_TYPE_TUPLE_STAGECONFIG_CYCLES_PER_HOUR         0   //!< Element Index
#define STORM_TYPE_TUPLE_STAGECONFIG_MIN_ON_TIME             1   //!< Element Index
#define STORM_TYPE_TUPLE_STAGECONFIG_MIN_OFF_TIME            2   //!< Element Index
#define STORM_TYPE_TUPLE_STAGECONFIG_CAPACITY_PERCENT        3   //!< Element Index
#define STORM_TYPE_TUPLE_STAGECONFIG_NUM_TUPLES              (STORM_TYPE_TUPLE_STAGECONFIG_CAPACITY_PERCENT+1) //!< Number of Elements



/// Namespaces
namespace Storm
    {
    namespace Type
        {
        namespace Tuple
            {
            /** Tuple for the User operating mode, setpoints, fan mode, etc. a thermostat
             */
            class StageConfig : public Rte::Tuple::Basic<STORM_TYPE_TUPLE_STAGECONFIG_NUM_TUPLES>
                {
                public:
                    /// Number of on/off Cycles Per hours (at 50% load)
                    Rte::Element::Uinteger8   m_cyclesPerHours;

                    /// Min on time (in seconds)
                    Rte::Element::Uinteger16  m_minOnTime;

                    /// Min off time (in seconds)
                    Rte::Element::Uinteger16  m_minOffTime;

                    /// This stage's capacity in terms of percent (0.0 - 1.0) of the total capacity for this stage's cooling/heating source.  The sum of ALL stages for ALL sources should equal N where N is the number of sources.
                    Rte::Element::Float       m_capacityPercent;


                public:
                    /// Constructor
                    StageConfig( void )
                        {
                        registerElement( STORM_TYPE_TUPLE_STAGECONFIG_CYCLES_PER_HOUR, m_cyclesPerHours );
                        registerElement( STORM_TYPE_TUPLE_STAGECONFIG_MIN_ON_TIME, m_minOnTime );
                        registerElement( STORM_TYPE_TUPLE_STAGECONFIG_MIN_OFF_TIME, m_minOffTime );
                        registerElement( STORM_TYPE_TUPLE_STAGECONFIG_CAPACITY_PERCENT, m_capacityPercent );
                        }

                };


            };      // end namespace
        };
    };
#endif  // end header latch
