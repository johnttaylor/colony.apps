#ifndef Storm_Component_OperatingMode_h_
#define Storm_Component_OperatingMode_h_
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
#include "Storm/Component/Base.h"
#include "Storm/Type/Element/TMode.h"
#include "Storm/Type/Element/OMode.h"
#include "Storm/Type/Element/Pulse.h"
#include "Rte/Element/ElapsedPrecisionTime.h"
#include "Cpl/Type/SeqNumber.h"


/** This constant defines the negative cooling offset (in degrees 'F) for
    preferring cooling operating mode over heating mode.
 */
#ifndef OPTION_STORM_COMPONENT_OPERATING_MODE_COOLING_OFFSET
#define OPTION_STORM_COMPONENT_OPERATING_MODE_COOLING_OFFSET        1.0f
#endif   

 /** This constant defines the time hystersis, in seconds, for how long the
     system must be off (no active heating/cooling) before allow a change in
     the Operating Mode when the User mode is in AUTO.  The default is
     10 minutes.
  */
#ifndef OPTION_STORM_COMPONENT_OPERATING_MODE_SECONDS_HYSTERESIS
#define OPTION_STORM_COMPONENT_OPERATING_MODE_SECONDS_HYSTERESIS    (10 * 60)
#endif   


  /// Namespaces
namespace Storm
    {
    namespace Component
        {
        /** This concrete class determine the actual mode of operation for the
            thermostat when the user has selected "Auto Mode".  It is also responsible
            for determine the 'active' setpoint and calculation the delta error value
            for use by the PI Component.
         */
        class OperatingMode : public Base
            {
            protected:
                /// See Constructors args...
                Rte::Element::Float&                    mi_coolingSetpoint;
                /// See Constructors args...
                Rte::Element::Float&                    mi_heatingSetpoint;
                /// See Constructors args...
                Storm::Type::Element::TMode&            mi_userMode;
                /// See Constructors args...
                Rte::Element::Float&                    mi_idt;
                /// See Constructors args...
                Rte::Element::Integer32&                mi_freezePiRefCount;
                /// See Constructors args...
                Rte::Element::ElapsedPrecisionTime&     mi_beginOffTime;
                /// See Constructors args...
                Storm::Type::Element::Pulse&            mi_resetPi;
                /// See Constructors args...
                Rte::Element::Boolean&                  mi_systemOn;

                /// See Constructors args...
                Rte::Element::Integer32&                mo_freezePiRefCount;
                /// See Constructors args...
                Storm::Type::Element::OMode&            mo_opMode;
                /// See Constructors args...
                Storm::Type::Element::Pulse&            mo_resetPi;
                /// See Constructors args...
                Storm::Type::Element::Pulse&            mo_opModeChanged;

            public:
                /// Constructor
                OperatingMode( Rte::Element::Float&                    i_coolingSetpoint,  //!< Input: Cooling setpoint in degrees Fahrenheit
                               Rte::Element::Float&                    i_heatingSetpoint,  //!< Input: Heating setpoint in degrees Fahrenheit
                               Storm::Type::Element::TMode&            i_userMode,         //!< Input: The thermostat mode to be resolved
                               Rte::Element::Float&                    i_idt,              //!< Input: The current indoor temperature in degrees Fahrenheit
                               Rte::Element::Integer32&                i_freezePiRefCount, //!< Input: Current/Pass-through freeze-the-PI-controller reference counter
                               Rte::Element::ElapsedPrecisionTime&     i_beginOffTime,     //!< Input: The elasped time marker of when the system turned off all active Cooling/Heating
                               Storm::Type::Element::Pulse&            i_resetPi,          //!< Input: Current/Pass-through reset-the-PI-controller request
                               Rte::Element::Boolean&                  i_systemOn,         //!< Input: Indicates that system is actively Cooling or Heating
                               Rte::Element::Integer32&                o_freezePiRefCount, //!< Output: Potentially new freeze-the-PI-controller reference counter (when the operating mode transitions to off, the algorithm will freeze the PI controller and reset the controller; then unfreezes on a transition to non-off mode)
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
                bool execute( Cpl::System::ElaspedTime::Precision_T currentTick,
                              Cpl::System::ElaspedTime::Precision_T currentInterval
                            );


            public:
                /// See Storm::Component::Api
                bool start( Cpl::System::ElaspedTime::Precision_T intervalTime );


            protected:
                /// Helper method
                virtual void setNewOMode( Storm::Type::Element::OMode_T::Enum newOMode );
            };



        };      // end namespace
    };      // end namespace
#endif  // end header latch


