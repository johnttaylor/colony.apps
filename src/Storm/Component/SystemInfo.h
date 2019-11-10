#ifndef Storm_Component_SystemInfo_h_
#define Storm_Component_SystemInfo_h_
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
#include "Storm/Dm/MpIduConfig.h"
#include "Storm/Dm/MpOduConfig.h"
#include "Storm/Dm/MpSimpleAlarm.h"
#include "Storm/Dm/MpAllowedOperatingModes.h"
#include "Storm/Dm/MpSystemType.h"
#include "Cpl/Dm/Mp/RefCounter.h"




/// Namespaces
namespace Storm {
/// Namespaces
namespace Component {


/** This concrete class determines System Attributes.
 */
class SystemInfo : public Base
{
public:
    /// Input Model Points
    struct Input_T
    {
        Storm::Dm::MpIduConfig&         iduConfig;              //!< Indoor Unit configuration
        Storm::Dm::MpOduConfig&         oduConfig;              //!< Outdoor Unit configuration
    };


    /// Output Parameters
    struct Output_T
    {
        Storm::Dm::MpAllowedOperatingModes& allowedModes;               //!< Specifies what the system is physically capable of doing (e.g. a furnace only system can not perform active cooling)
        Storm::Dm::MpSimpleAlarm&           noActiveConditioningAlarm;  //!< Alarm MP used indicate that system configuration does NOT provide any active conditional (i.e. no heating and no cooling capacity)
        Storm::Dm::MpSystemType&            systemType;                 //!< The current system configuration/type based on the current indoor/outdoor equipment settings
        Cpl::Dm::Mp::RefCounter&            systemForcedOffRefCnt;	    //!< Reference Counter: When greater the zero the system is required to be forced off.
    };

    /// Component specific initialization
    bool start( Cpl::System::ElapsedTime::Precision_T newInterval );

protected:
    /// My Model Point inputs
    struct Input_T  m_in;

    /// My Model Point outputs
    struct Output_T m_out;


public:
    ///Constructor.  
    SystemInfo( struct Input_T inputs, struct Output_T outputs );


protected:
    /// See Storm::Component::Base
    bool execute( Cpl::System::ElapsedTime::Precision_T currentTick,
                  Cpl::System::ElapsedTime::Precision_T currentInterval );
};



};      // end namespace
};      // end namespace
#endif  // end header latch
