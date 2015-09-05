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


/** Tuple for the Public operating mode, setpoints, fan mode, etc. a thermostat
 */
class Operate: public Rte::Tuple::Basic<5>
{
public:
    /// Definition for RTE Element type for thermostat mode
    typedef Basic<Storm::Type::TMode_T, Storm::Type::RTE_DATATYPE_TMODE_T>     TMode_T;


public:
    /// Element index for: Cooling setpoint
    static const unsigned IDX_COOL_SETPOINT   = 0;

    /// Element index for: HEATING setpoint
    static const unsigned IDX_HEAT_SETPOINT   = 1;

    /// Element index for: Mode
    static const unsigned IDX_MODE            = 2;

    /// Element index for: Mode
    static const unsigned IDX_FAN_CONT        = 3;

    /// Element index for: No Primary
    static const unsigned IDX_NO_PRIMARY_HEAT = 4;


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
        registerElement( IDX_COOL_SETPOINT,   m_coolSetpoint );
        registerElement( IDX_HEAT_SETPOINT,   m_heatSetpoint );
        registerElement( IDX_MODE,            m_mode );
        registerElement( IDX_FAN_CONT,        m_fanCont );
        registerElement( IDX_NO_PRIMARY_HEAT, m_noPrimaryHeat );
        }

};


};      // end namespace
};      // end namespace
#endif  // end header latch
