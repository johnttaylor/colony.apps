#ifndef Storm_Rte_Tuple_UserConfig_h_
#define Storm_Rte_Tuple_UserConfig_h_
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
#include "Storm/Rte/DehumOption.h"


/// Element index for: Cooling setpoint
#define STORM_RTE_TUPLE_USER_CONFIG_DEHUM_OPTION            0

/// Element index for: HEATING setpoint
#define STORM_RTE_TUPLE_USER_CONFIG_DEHUM_SETPOINT          1

/// Element index for: Mode
#define STORM_RTE_TUPLE_USER_CONFIG_AIR_FILTER_TIME         2

/// Element index for: Deadband
#define STORM_RTE_TUPLE_USER_CONFIG_DEADBAND                3

/// Element index for: Auto Mode Enabled
#define STORM_RTE_TUPLE_USER_CONFIG_AUTO_MODE_ENABLED       4

/// Element index for: Fast cooling Enabled
#define STORM_RTE_TUPLE_USER_CONFIG_FAST_COOLING_ENABLED    5

/// Element index for: Fast heating Enabled
#define STORM_RTE_TUPLE_USER_CONFIG_FAST_HEATING_ENABLED    6

/// Number of Tuples in the Point
#define STORM_RTE_TUPLE_USER_CONFIG_NUM_TUPLES              (STORM_RTE_TUPLE_USER_CONFIG_FAST_HEATING_ENABLED+1)



/// Namespaces
namespace Storm { namespace Rte { namespace Tuple {


/** Tuple for the User configurable parameters for a thermostat
 */
class UserConfig: public Rte::Tuple::Basic<STORM_RTE_TUPLE_USER_CONFIG_NUM_TUPLES>
{
public: 
    /// Enable/disable dehumidify algorithms
    Storm::Rte::DehumOption         m_dehumOption;

    /// Dehumidify Setpoint (% humidity)
    Rte::Element::Float_T           m_dehumSetpoint;

    /// Elasped Time (in days) between Air filter changes
    Rte::Element::Uinteger16_T      m_airFilterTime;

    /// Deadband (in degrees Fahrenheit) between the Cooling/Heating setpoint when in AUTO mode
    Rte::Element::Float_T           m_deadband;

    /// Enable/disable the use of "Auto" thermostat mode
    Rte::Element::Boolean_T         m_autoModeEnabled;

    /// Enables faster PI control for cooling operation
    Rte::Element::Boolean_T         m_fastCoolingEnabled;

    /// Enables faster PI control for heating operation
    Rte::Element::Boolean_T         m_fastHeatingEnabled;


public:
    /// Constructor
    UserConfig( void )
        {
        registerElement( STORM_RTE_TUPLE_USER_CONFIG_DEHUM_OPTION,         m_dehumOption        );
        registerElement( STORM_RTE_TUPLE_USER_CONFIG_DEHUM_SETPOINT,       m_dehumSetpoint      );
        registerElement( STORM_RTE_TUPLE_USER_CONFIG_AIR_FILTER_TIME,      m_airFilterTime      );
        registerElement( STORM_RTE_TUPLE_USER_CONFIG_DEADBAND,             m_deadband           );
        registerElement( STORM_RTE_TUPLE_USER_CONFIG_AUTO_MODE_ENABLED,    m_autoModeEnabled    );
        registerElement( STORM_RTE_TUPLE_USER_CONFIG_FAST_COOLING_ENABLED, m_fastCoolingEnabled );
        registerElement( STORM_RTE_TUPLE_USER_CONFIG_FAST_HEATING_ENABLED, m_fastHeatingEnabled );
        }

};


};      // end namespace
};      
};      
#endif  // end header latch
