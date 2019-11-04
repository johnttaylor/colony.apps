#ifndef Storm_Component_FanControl_h_
#define Storm_Component_FanControl_h_
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

#include "Storm/Component/Base.h"
#include "Cpl/Dm/Mp/Float.h"
#include "Cpl/Dm/Mp/Bool.h"
#include "Cpl/Dm/Mp/RefCounter.h"
#include "Storm/Dm/MpFanMode.h"
#include "Storm/Dm/MpSystemType.h"
#include "Storm/Dm/MpVirtualOutputs.h"
#include "Storm/Dm/MpEquipmentBeginTimes.h"
#include "Storm/Dm/MpComfortConfig.h"
#include "Storm/Dm/MpIduConfig.h"


/** The fan continuous blower speed (as percent speed: n/1000) when the indoor 
    unit has a variable speed blower
 */
#ifndef OPTION_STORM_COMPONENT_FAN_CONTROL_CONTINUOUS_SPEED
#define OPTION_STORM_COMPONENT_FAN_CONTROL_CONTINUOUS_SPEED     500 // Half speed
#endif

/// Namespaces
namespace Storm
{
/// Namespaces
namespace Component
{
/** This class is responsible for implementing the Indoor Fan Policies (e.g.
    Fan Continuous operation)
 */
class FanControl : public Base
{
public:
    /// Input Model Points
    struct Input_T
    {
        Storm::Dm::MpFanMode&               fanMode;                //!< The current fan operation/mode as selected by the user
        Storm::Dm::MpIduConfig&             iduConfig;              //!< Indoor Unit configuration
        Storm::Dm::MpComfortConfig&         comfortConfig;          //!< The Comfort configuration settings
        Storm::Dm::MpSystemType&            systemType;             //!< The current system configuration/type based on the current indoor/outdoor equipment settings
        Storm::Dm::MpVirtualOutputs&        vOutputs;               //!< The virtual system outputs
        Storm::Dm::MpEquipmentBeginTimes&   equipmentBeginTimes;    //!< The begin times for when the HVAC outputs turned on/off
        Cpl::Dm::Mp::RefCounter&            systemForcedOffRefCnt;	//!< Reference Counter: When greater the zero the system is required to be forced off.
    };


    /// Output Model Points
    struct Output_T
    {
        Storm::Dm::MpVirtualOutputs&        vOutputs;               //!< The virtual system outputs
    };


public:
    /// Constructor
    FanControl( struct Input_T ins, struct Output_T outs );

    /// See Storm::Component::Api
    bool start( Cpl::System::ElapsedTime::Precision_T intervalTime );

protected:
    /// See Storm::Component::Base
    bool execute( Cpl::System::ElapsedTime::Precision_T currentTick,
                  Cpl::System::ElapsedTime::Precision_T currentInterval );

protected:
    /// My Model Point inputs
    struct Input_T  m_in;

    /// My Model Point outputs
    struct Output_T m_out;
};



};      // end namespace
};      // end namespace
#endif  // end header latch
