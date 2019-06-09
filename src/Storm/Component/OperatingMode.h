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
#include "Cpl/Dm/Mp/Float.h"
#include "Cpl/Dm/Mp/RefCounter.h"


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


/// Namespaces
namespace Storm {
/// Namespaces
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
        Storm::Dm::MpSetpoints*      setpoints;         //!< Cooling & Heating set-points in degrees Fahrenheit
        Storm::Dm::MpThermostatMode* userMode;          //!< The thermostat mode to be resolved
        Cpl::Dm::Mp::Float*          idt;               //!< The current indoor temperature in degrees Fahrenheit
        Cpl::Dm::Mp::RefCounter      freezePiRefCnt;    //!< Current/Pass-through freeze-the-PI-controller reference counter

            Rte::Element::ElapsedPrecisionTime&     i_beginOffTime,     //!< Input: The elasped time marker of when the system turned off all active Cooling/Heating
            Storm::Type::Element::Pulse&            i_resetPi,          //!< Input: Current/Pass-through reset-the-PI-controller request
            Rte::Element::Boolean&                  i_systemOn,         //!< Input: Indicates that system is actively Cooling or Heating
            
            Cpl::Dm::Mp::Float*      primaryIdt;                //!< Current indoor temperature in degrees Fahrenheit of the PRIMARY IDT sensor (aka the onboard sensor)
        Cpl::Dm::Mp::Float*      secondaryIdt;              //!< Current indoor temperature in degrees Fahrenheit of the SECONDARY IDT sensor (aka the remote sensor)
        Cpl::Dm::Mp::Bool*       haveSecondaryIdt;			//!< Indicates that Secondary IDT sensor is available
        Cpl::Dm::Mp::RefCounter* systemForcedOffRefCnt;    //!< Reference Counter: When greater the zero the system is required to be forced off.
    };


    /// Output Parameters
    struct Output_T
    {
        Cpl::Dm::Mp::Float*      activeIdt;				    //!< Current indoor temperature in degrees Fahrenheit to be used for the algorithm
        Cpl::Dm::Mp::RefCounter* systemForcedOffRefCnt;	//!< Reference Counter: When greater the zero the system is required to be forced off.
        Storm::Dm::MpIdtAlarm*   idtAlarms;                 //!< Indoor Temperature Alarms
    };

public:
    /// Constructor
    OperatingMode(                    Rte::Element::Integer32&                o_freezePiRefCount, //!< Output: Potentially new freeze-the-PI-controller reference counter (when the operating mode transitions to off, the algorithm will freeze the PI controller and reset the controller; then unfreezes on a transition to non-off mode)
                   Storm::Type::Element::OMode&            o_opMode,           //!< Output: Actual/Operating mode for the thermostat
                   Storm::Type::Element::Pulse&            o_resetPi,          //!< Output: Potentially a reset-the-PI-controller request (on a mode change this class will reset the PI component)
                   Storm::Type::Element::Pulse&            o_opModeChanged     //!< Output: Indicates that there is/was an operating mode transition
    );

protected:
    /// Current/Previous operating mode
    Storm::Type::Element::OMode_T::Enum  m_prevOperatingMode;

    /// Flag used to detect the transition to AUTO mode
    bool                                 m_inAuto;



protected:
    /// See Storm::Component::Base
    bool execute( Cpl::System::ElapsedTime::Precision_T currentTick,
                  Cpl::System::ElapsedTime::Precision_T currentInterval
    );


public:
    /// See Storm::Component::Api
    bool start( Cpl::System::ElapsedTime::Precision_T intervalTime );


protected:
    /// Helper method
    virtual void setNewOMode( Storm::Type::Element::OMode_T::Enum newOMode );
};



};      // end namespace
};      // end namespace
#endif  // end header latch


