#ifndef Storm_Component_Pi_h_
#define Storm_Component_Pi_h_
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
#include "Cpl/Type/SeqNumber.h"
#include "Storm/Type/Element/Pulse.h"


/// Namespaces
namespace Storm
    {
    namespace Component
        {
        /** This concrete class implements a simple (P)roportional (I)ntegral
            controller that only allows the integral term to be a positive number. This
            class is simplification of a basic PID Controller (see https://en.wikipedia.org/wiki/PID_controller)
            The class has the following 'integral wind-up' protections:

                o Allows many external entities to explicitly inhibit the integral term.

                o The output of PI is clamped based on the configuration parameter
                  m_maxOutValue.  When the output is clamped the integral term is
                  inhibited.

                o The integral term is clamped such that integral term by itself
                  (i.e. with zero error) does not exceed the maximum configured
                  output of the PI.
         */
        class Pi : public Base
            {
            protected:
                /// See Constructor args
                Rte::Element::Float&            mi_deltaError;
                /// See Constructor args
                Rte::Element::Integer32&        mi_freezeRefCount;
                /// See Constructor args
                Rte::Element::Integer32&        mi_inhibitRefCount;
                /// See Constructor args
                Rte::Element::Float&            mi_gain;
                /// See Constructor args
                Rte::Element::Float&            mi_resetTime;
                /// See Constructor args
                Rte::Element::Float&            mi_maxOutValue;
                /// See Constructor args
                Storm::Type::Element::Pulse&    mi_reset;

                /// See Constructor args
                Rte::Element::Float&            mo_out;
                /// See Constructor args
                Rte::Element::Float&            mo_sumError;
                /// See Constructor args
                Rte::Element::Boolean&          mo_inhibitedState;

            public:
                /// Constructor
                Pi( Rte::Element::Float&          i_deltaError,       //!< Input: Delta error between a setpoint (SP) and the process variable (PV).  A positive error increases the output (OUT)
                    Rte::Element::Integer32&      i_freezeRefCount,   //!< Input: Reference counter for external entities to freeze the OUT value (also inhibits the integral term). If the counter is greater than zero, then a new OUT value is calculated.
                    Rte::Element::Integer32&      i_inhibitRefCount,  //!< Input: Reference counter for external entities to inhibit the integral term from changing.  If the counter is greater than zero, then the integral is inhibited
                    Rte::Element::Float&          i_gain,             //!< Input: The proportional gain constant for the PI
                    Rte::Element::Float&          i_resetTime,        //!< Input: Reset time in milliseconds. The reset time is the time it takes for the intergral term, given a constant error, to effect PI OUT term the same amount as the Proportional gain.
                    Rte::Element::Float&          i_maxOutValue,      //!< Input: The maximum allowed OUT value.  The controller will not output a value greater than 'maxOutValue'.  Also the controller ensures that the Integral term, by itself, never exceeds 'maxOutValue'.
                    Storm::Type::Element::Pulse&  i_reset,            //!< Input: Request to reset the Controller, When this flag is true, the Controller resets its output and integral term to zero.
                    Rte::Element::Float&          o_out,              //!< Output: Output of the PI Controller.  This is unitless positive number that ranges from 0.0 to m_maxOutValue.
                    Rte::Element::Float&          o_sumError,         //!< Output: New sum error term
                    Rte::Element::Boolean&        o_inhibitedState    //!< Output: This flag is true if the PI Controller's integral is currently inhibited. Note: This flag includes any internal inhibiting of the integral term as well as being set true when the PI 'freezed'
                  );

            public:
                /// See Storm::Component::Api
                bool start( Cpl::System::ElapsedTime::Precision_T intervalTime );

            protected:
                // dt interval time - in milliseconds - as a float (instead of Precision_T struct)
                float m_dt;

                // Previous sum error term
                float m_prevSumError;

                // Previous OUT value
                float m_prevOut;


            protected:
                /// See Storm::Component::Base
                bool execute( Cpl::System::ElapsedTime::Precision_T currentTick,
                              Cpl::System::ElapsedTime::Precision_T currentInterval
                            );


            };



        };      // end namespace
    };      // end namespace
#endif  // end header latch
