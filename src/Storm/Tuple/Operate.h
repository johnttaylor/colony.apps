#ifndef Storm_Tuple_Operate_h_
#define Storm_Tuple_Operate_h_
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
#include "Storm/Type.h"


/// Namespaces
namespace Storm { namespace Tuple {



/// Element index for: Cooling setpoint
#define STORM_TUPLE_OPERATE_COOL_SETPOINT       0

/// Element index for: HEATING setpoint
#define STORM_TUPLE_OPERATE_HEAT_SETPOINT       1

/// Element index for: Mode
#define STORM_TUPLE_OPERATE_MODE                2

/// Element index for: Mode
#define STORM_TUPLE_OPERATE_FAN_CONT            3

/// Element index for: No Primary
#define STORM_TUPLE_OPERATE_NO_PRIMARY_HEAT     4

/// Number of Tuples in the Point
#define STORM_TUPLE_OPERATE_NUM_TUPLES          (STORM_TUPLE_OPERATE_NO_PRIMARY_HEAT+1)



/** Tuple for the User operating mode, setpoints, fan mode, etc. a thermostat
 */
class Operate: public Rte::Tuple::Basic<STORM_TUPLE_OPERATE_NUM_TUPLES>
{
public:
    /// Definition for RTE Element type for thermostat mode
    typedef Basic<Storm::Type::TMode::Enum_T, Storm::Type::Rte::DATATYPE_TMODE_T>  TMode_T;


public: 
    /// Cooling Setpoint in degrees Fahrenheit
    Rte::Element::Float_T   m_coolSetpoint;

    /// Heating Setpoint in degrees Fahrenheit
    Rte::Element::Float_T   m_heatSetpoint;

    /// Mode of operation for the thermostat
    TMode_T                 m_mode;

    /// Indoor fan operation mode (Auto vs. Continuous)
    Rte::Element::Boolean_T m_fanCont;

    /// Flag that indicates heating should be done using "Emergency Heat", e.g. no heatpump operation
    Rte::Element::Boolean_T m_noPrimaryHeat;


public:
    /// Constructor
    Operate( void )
        {
        registerElement( STORM_TUPLE_OPERATE_COOL_SETPOINT,   m_coolSetpoint );
        registerElement( STORM_TUPLE_OPERATE_HEAT_SETPOINT,   m_heatSetpoint );
        registerElement( STORM_TUPLE_OPERATE_MODE,            m_mode );
        registerElement( STORM_TUPLE_OPERATE_FAN_CONT,        m_fanCont );
        registerElement( STORM_TUPLE_OPERATE_NO_PRIMARY_HEAT, m_noPrimaryHeat );
        }

};


};      // end namespace
};      // end namespace
#endif  // end header latch
