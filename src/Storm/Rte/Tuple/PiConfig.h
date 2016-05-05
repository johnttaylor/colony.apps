#ifndef Storm_Rte_Tuple_PiConfig_h_
#define Storm_Rte_Tuple_PiConfig_h_
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

#include "Rte/Tuple/Basic.h"
#include "Rte/Element/Basic.h"
#include "Storm/Rte/TMode.h"




#define STORM_RTE_TUPLE_PICONFIG_COOLING_GAIN_0         0   //!< Element Index
#define STORM_RTE_TUPLE_PICONFIG_COOLING_RESET_TIME_0   1   //!< Element Index
#define STORM_RTE_TUPLE_PICONFIG_HEATING_GAIN_0         2   //!< Element Index
#define STORM_RTE_TUPLE_PICONFIG_HEATING_RESET_TIME_0   3   //!< Element Index
#define STORM_RTE_TUPLE_PICONFIG_COOLING_GAIN_1         4   //!< Element Index
#define STORM_RTE_TUPLE_PICONFIG_COOLING_RESET_TIME_1   5   //!< Element Index
#define STORM_RTE_TUPLE_PICONFIG_HEATING_GAIN_1         6   //!< Element Index
#define STORM_RTE_TUPLE_PICONFIG_HEATING_RESET_TIME_1   7   //!< Element Index
#define STORM_RTE_TUPLE_PICONFIG_NUM_TUPLES             (STORM_RTE_TUPLE_PICONFIG_HEATING_RESET_TIME_1+1) //!< Number of Elements



/// Namespaces
namespace Storm { namespace Rte { namespace Tuple {


/** Tuple for the User operating mode, setpoints, fan mode, etc. a thermostat
 */
class PiConfig: public ::Rte::Tuple::Basic<STORM_RTE_TUPLE_PICONFIG_NUM_TUPLES>
{
public: 
    /// PI Constants Set#0: Cooling Proportional gain
    ::Rte::Element::Float_T       m_coolingGain0;

    /// PI Constants Set#0: Cooling Reset time (in milliseconds)
    ::Rte::Element::Float_T       m_coolingResetTime0;

    /// PI Constants Set#0: Heating Proportional gain
    ::Rte::Element::Float_T       m_heatingGain0;

    /// PI Constants Set#0: Heating Reset time (in milliseconds)
    ::Rte::Element::Float_T       m_heatingResetTime0;

    /// PI Constants Set#1: Cooling Proportional gain
    ::Rte::Element::Float_T       m_coolingGain1;

    /// PI Constants Set#1: Cooling Reset time (in milliseconds)
    ::Rte::Element::Float_T       m_coolingResetTime1;

    /// PI Constants Set#1: Heating Proportional gain
    ::Rte::Element::Float_T       m_heatingGain1;

    /// PI Constants Set#1: Heating Reset time (in milliseconds)
    ::Rte::Element::Float_T       m_heatingResetTime1;



public:
    /// Constructor
    PiConfig( void )
        {
        registerElement(STORM_RTE_TUPLE_PICONFIG_COOLING_GAIN_0,       m_coolingGain0      );
        registerElement(STORM_RTE_TUPLE_PICONFIG_COOLING_RESET_TIME_0, m_coolingResetTime0 );
        registerElement(STORM_RTE_TUPLE_PICONFIG_HEATING_GAIN_0,       m_heatingGain0      );
        registerElement(STORM_RTE_TUPLE_PICONFIG_HEATING_RESET_TIME_0, m_heatingResetTime0 );
        registerElement(STORM_RTE_TUPLE_PICONFIG_COOLING_GAIN_1,       m_coolingGain1      );
        registerElement(STORM_RTE_TUPLE_PICONFIG_COOLING_RESET_TIME_1, m_coolingResetTime1 );
        registerElement(STORM_RTE_TUPLE_PICONFIG_HEATING_GAIN_1,       m_heatingGain1      );
        registerElement(STORM_RTE_TUPLE_PICONFIG_HEATING_RESET_TIME_1, m_heatingResetTime1 );
        }                                                             

};


};      // end namespace
};     
};     
#endif  // end header latch
