#ifndef Storm_Tuple_UserConfig_h_
#define Storm_Tuple_UserConfig_h_
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
#include "Storm/Type/Rte.h"


/// Element index for: Cooling setpoint
#define STORM_TUPLE_USER_CONFIG_DEHUM_OPTION        0

/// Element index for: HEATING setpoint
#define STORM_TUPLE_USER_CONFIG_DEHUM_SETPOINT      1

/// Element index for: Mode
#define STORM_TUPLE_USER_CONFIG_AIR_FILTER_TIME     2

/// Element index for: Mode
#define STORM_TUPLE_USER_CONFIG_DEADBAND            3

/// Number of Tuples in the Point
#define STORM_TUPLE_USER_CONFIG_NUM_TUPLES          (STORM_TUPLE_USER_CONFIG_DEADBAND+1)



/// Namespaces
namespace Storm { namespace Tuple {


/** Tuple for the User configurable parameters for a thermostat
 */
class UserConfig: public Rte::Tuple::Basic<STORM_TUPLE_USER_CONFIG_NUM_TUPLES>
{
public: 
    /// Enable/disable dehumidify algorithms
    Storm::Type::Rte::DehumOption_T m_dehumOption;

    /// Dehumidify Setpoint (% humidity)
    Rte::Element::Float_T           m_dehumSetpoint;

    /// Elasped Time (in days) between Air filter changes
    Rte::Element::Uinteger16_T      m_airFilterTime;

    /// Deadband (in degrees Fahrenheit) between the Cooling/Heating setpoint when in AUTO mode
    Rte::Element::Float_T           m_deadband;


public:
    /// Constructor
    UserConfig( void )
        {
        registerElement( STORM_TUPLE_USER_CONFIG_DEHUM_OPTION,    m_dehumOption   );
        registerElement( STORM_TUPLE_USER_CONFIG_DEHUM_SETPOINT,  m_dehumSetpoint );
        registerElement( STORM_TUPLE_USER_CONFIG_AIR_FILTER_TIME, m_airFilterTime );
        registerElement( STORM_TUPLE_USER_CONFIG_DEADBAND,        m_deadband      );
        }

};


};      // end namespace
};      
#endif  // end header latch
