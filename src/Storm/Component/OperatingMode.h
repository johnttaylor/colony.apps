#ifndef Storm_Component_OperatingMode_h_
#define Storm_Component_OperatingMode_h_
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

#include "colony_config.h"
#include "Storm/Component/Base.h"
#include "Storm/Dm/MpSetpoints.h"
#include "Storm/Dm/MpOperatingMode.h"
#include "Storm/Dm/MpThermostatMode.h"
#include "Storm/Dm/MpSimpleAlarm.h"
#include "Storm/Dm/MpAllowedOperatingModes.h"
#include "Storm/Dm/MpSystemType.h"
#include "Cpl/Dm/Mp/Float.h"
#include "Cpl/Dm/Mp/Bool.h"
#include "Cpl/Dm/Mp/RefCounter.h"
#include "Cpl/Dm/Mp/ElapsedPrecisionTime.h"


/** This constant defines the negative cooling offset (in degrees 'F) for
    preferring cooling operating mode over heating mode.
 */
#ifndef OPTION_STORM_COMPONENT_OPERATING_MODE_COOLING_OFFSET
#define OPTION_STORM_COMPONENT_OPERATING_MODE_COOLING_OFFSET        1.0f
#endif   

 /** This constant defines the minimum separation (in degrees 'F) between the
     cooling and heating set-points when the User has selected 'Auto' mode.
   */
#ifndef OPTION_STORM_COMPONENT_OPERATING_MODE_MIN_SETPOINT_DELTA
#define OPTION_STORM_COMPONENT_OPERATING_MODE_MIN_SETPOINT_DELTA    4.0f
#endif   

   /** This constant defines the time hysteresis, in seconds, for how long the
       system must be off (no active heating/cooling) before allow a change in
       the Operating Mode when the User mode is in AUTO.  The default is
       10 minutes.
    */
#ifndef OPTION_STORM_COMPONENT_OPERATING_MODE_SECONDS_HYSTERESIS
#define OPTION_STORM_COMPONENT_OPERATING_MODE_SECONDS_HYSTERESIS    (10 * 60)
#endif   


/// 
namespace Storm {
/// 
namespace Component {

/** This concrete class determine the actual mode of operation for the
    thermostat when the user has selected "Auto Mode".  It is also responsible
    for determine the 'active' set-point and calculation the delta error value
    for use by the PI Component.
 */
class OperatingMode : public Base
{
public:
    /// Input Model Points
    struct Input_T
    {
        Storm::Dm::MpSetpoints&             setpoints;              //!< Cooling & Heating set-points in degrees Fahrenheit
        Storm::Dm::MpThermostatMode&        userMode;               //!< The thermostat mode to be resolved
        Cpl::Dm::Mp::Float&                 idt;                    //!< The current indoor temperature in degrees Fahrenheit
        Cpl::Dm::Mp::ElapsedPrecisionTime&  beginOffTime;           //!< The elapsed time marker of when the system turned off all active Cooling/Heating
        Cpl::Dm::Mp::Bool&                  systemOn;               //!< Indicates that system is actively Cooling or Heating
        Storm::Dm::MpAllowedOperatingModes& allowedModes;           //!< Input to what the system is physically capable of doing (e.g. a furnace only system can not perform active cooling)
        Cpl::Dm::Mp::RefCounter&            systemForcedOffRefCnt;	//!< Reference Counter: When greater the zero the system is required to be forced off.
        Storm::Dm::MpSystemType&            systemType;             //!< The current system configuration/type based on the current indoor/outdoor equipment settings
    };


    /// Output Parameters
    struct Output_T
    {
        Storm::Dm::MpOperatingMode&         operatingMode;          //!< The actual operating thermostat mode (derived from the User mode setting)
        Cpl::Dm::Mp::Bool&                  operatingModeChanged;   //!< When true, indicates that the operating mode changed during the processing; else the output is set to false
        Cpl::Dm::Mp::Bool&                  pulseResetPi;           //!< Triggers a reset-the-PI-controller request
        Storm::Dm::MpSimpleAlarm&           userConfigModeAlarm;    //!< Alarm MP used indicate that user mode is not compatible with the allowed modes operation for the system
    };

public:
    /// Constructor
    OperatingMode( struct Input_T ins, struct Output_T outs );

    /// Component specific initialization
    bool start( Cpl::System::ElapsedTime::Precision_T newInterval );

protected:
    /// Inputs
    struct Input_T              m_in;

    /// Outputs
    struct Output_T             m_out;

    /// Current/Previous operating mode (because of limitations of BETTER_ENUM - when used inside a class - we use a simply integer to hold the enum value)
    int                         m_prevOperatingMode;

    /// Flag used to detect the transition to AUTO mode
    bool                        m_inAuto;



protected:
    /// See Storm::Component::Base
    bool execute( Cpl::System::ElapsedTime::Precision_T currentTick,
                  Cpl::System::ElapsedTime::Precision_T currentInterval );


protected:
    /// Helper method
    virtual void setNewOperatingMode( Storm::Type::OperatingMode newOpMode, Storm::Type::SystemType systemType );
};



};      // end namespace
};      // end namespace
#endif  // end header latch


