#ifndef Storm_Thermostat_ddtuples_h_
#define Storm_Thermostat_ddtuples_h_
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
#include "Rte/Element/Precision.h"
#include "Storm/Rte/TMode.h"
#include "Storm/Rte/OMode.h"
#include "Storm/Rte/Pulse.h"


/// Namespaces
namespace Storm { namespace Thermostat {


/////////////////////////////////////////////////////////////////////
// LOAD VALUE
/////////////////////////////////////////////////////////////////////
#define STORM_THERMOSTAT_TUPLE_LOADVALUE_VALUE                0  //!< Element index
#define STORM_THERMOSTAT_TUPLE_LOADVALUE_SUMERROR             1  //!< Element index
#define STORM_THERMOSTAT_TUPLE_LOADVALUE_DELTAERROR           2  //!< Element index
#define STORM_THERMOSTAT_TUPLE_LOADVALUE_FREEZEREFCOUNT       3  //!< Element index
#define STORM_THERMOSTAT_TUPLE_LOADVALUE_INHIBITREFCOUNT      4  //!< Element index
#define STORM_THERMOSTAT_TUPLE_LOADVALUE_RESET                5  //!< Element index
#define STORM_THERMOSTAT_TUPLE_LOADVALUE_INHIBITEDSTATE       6  //!< Element index
#define STORM_THERMOSTAT_TUPLE_LOADVALUE_GAIN                 7  //!< Element index
#define STORM_THERMOSTAT_TUPLE_LOADVALUE_RESETTIME            8  //!< Element index
#define STORM_THERMOSTAT_TUPLE_LOADVALUE_MAXVALUE             9  //!< Element index
#define STORM_THERMOSTAT_TUPLE_LOADVALUE_NUM_TUPLES           (STORM_THERMOSTAT_TUPLE_LOADVALUE_MAXVALUE+1) //!< Element count

/// Tuple
class TupleLoadValue: public ::Rte::Tuple::Basic<STORM_THERMOSTAT_TUPLE_LOADVALUE_MAXVALUE>
{
public: 
    ::Rte::Element::Float_T       m_value;                  //!< The current Load Value 
    ::Rte::Element::Float_T       m_sumError;               //!< Sum Error term associated with m_loadValue 
    ::Rte::Element::Float_T       m_deltaError;             //!< Delta Setpoint error used by the PI calculation
    ::Rte::Element::Integer32_T   m_freezeRefCount;         //!< Reference counter to freeze the output of the PI calculation
    ::Rte::Element::Integer32_T   m_inhibitRefCount;        //!< Reference counter to inhibit the PI's integral term
    Storm::Rte::Pulse             m_reset;                  //!< Pulse flag to request a reset of the PI block
    ::Rte::Element::Boolean_T     m_inhibitedState;         //!< Flag is true if PI's integral term is inhibited
    ::Rte::Element::Float_T       m_gain;                   //!< LoadValue Calculation: The proportional gain constant
    ::Rte::Element::Float_T       m_resetTime;              //!< LoadValue Calculation: Reset time in milliseconds. 
    ::Rte::Element::Float_T       m_maxValue;               //!< The maximum allowed Load Value.


public:
    /// Constructor
    TupleLoadValue( void )
        {
        registerElement( STORM_THERMOSTAT_TUPLE_LOADVALUE_VALUE,           m_value           );
        registerElement( STORM_THERMOSTAT_TUPLE_LOADVALUE_SUMERROR,        m_sumError        );
        registerElement( STORM_THERMOSTAT_TUPLE_LOADVALUE_DELTAERROR,      m_deltaError      );
        registerElement( STORM_THERMOSTAT_TUPLE_LOADVALUE_FREEZEREFCOUNT,  m_freezeRefCount  );
        registerElement( STORM_THERMOSTAT_TUPLE_LOADVALUE_INHIBITREFCOUNT, m_inhibitRefCount );
        registerElement( STORM_THERMOSTAT_TUPLE_LOADVALUE_RESET,           m_reset           );
        registerElement( STORM_THERMOSTAT_TUPLE_LOADVALUE_INHIBITEDSTATE,  m_inhibitedState  );
        registerElement( STORM_THERMOSTAT_TUPLE_LOADVALUE_GAIN,            m_gain            );
        registerElement( STORM_THERMOSTAT_TUPLE_LOADVALUE_RESETTIME,       m_resetTime       );
        registerElement( STORM_THERMOSTAT_TUPLE_LOADVALUE_MAXVALUE,        m_maxValue        );
        }    
        
    /// Resets all of my Pulse Elements
    void resetPulses( void )
        {
        m_reset.reset();
        }          

};


/////////////////////////////////////////////////////////////////////
// OPERATE
/////////////////////////////////////////////////////////////////////
#define STORM_THERMOSTAT_TUPLE_OPERATE_DELTASETPOINT      0  //!< Element index
#define STORM_THERMOSTAT_TUPLE_OPERATE_SETPOINTCHANGED    1  //!< Element index
#define STORM_THERMOSTAT_TUPLE_OPERATE_OPMODE             2  //!< Element index
#define STORM_THERMOSTAT_TUPLE_OPERATE_OPMODECHANGED      3  //!< Element index
#define STORM_THERMOSTAT_TUPLE_OPERATE_NUM_TUPLES           (STORM_THERMOSTAT_TUPLE_OPERATE_OPMODECHANGED+1) //!< Element count

/// Tuple
class TupleOperate: public ::Rte::Tuple::Basic<STORM_THERMOSTAT_TUPLE_OPERATE_NUM_TUPLES>
{
public:
    ::Rte::Element::Float_T   m_deltaSetpoint;                //!< Absolute value of the delta change of the active setpoint.  If m_setpointChanged is false, then this output has NO meaning (i.e. check m_setpointChanged before using this field) 
    Storm::Rte::Pulse         m_setpointChanged;              //!< Indicates if the active setpoint changed during THIS processing cycle.  This flag is NOT set on a change of the active setpoint.
    Storm::Rte::OMode         m_opMode;                       //!< Actual/Operating mode for the thermostat
    Storm::Rte::Pulse         m_opModeChanged;                //!< Indicates that there is/was an operating mode transition

public:
    /// Constructor
    TupleOperate( void )
        {
        registerElement( STORM_THERMOSTAT_TUPLE_OPERATE_DELTASETPOINT,   m_deltaSetpoint   );
        registerElement( STORM_THERMOSTAT_TUPLE_OPERATE_SETPOINTCHANGED, m_setpointChanged );
        registerElement( STORM_THERMOSTAT_TUPLE_OPERATE_OPMODE,          m_opMode          );
        registerElement( STORM_THERMOSTAT_TUPLE_OPERATE_OPMODECHANGED,   m_opModeChanged   );
        }              

    /// Resets all of my Pulse Elements
    void resetPulses( void )
        {
        m_setpointChanged.reset();
        m_opModeChanged.reset();
        }          

};


/////////////////////////////////////////////////////////////////////
// SYSTEM STATE
/////////////////////////////////////////////////////////////////////
#define STORM_THERMOSTAT_TUPLE_SYSSTATE_BEGINONTIME         0  //!< Element index
#define STORM_THERMOSTAT_TUPLE_SYSSTATE_BEGINOFFTIME        1  //!< Element index
#define STORM_THERMOSTAT_TUPLE_SYSSTATE_SYSTEMON            2  //!< Element index
#define STORM_THERMOSTAT_TUPLE_SYSSTATE_NUM_TUPLES          (STORM_THERMOSTAT_TUPLE_SYSSTATE_SYSTEMON+1) //!< Element count

/// Tuple
class TupleSysState: public ::Rte::Tuple::Basic<STORM_THERMOSTAT_TUPLE_SYSSTATE_NUM_TUPLES>
{
public:
    ::Rte::Element::Precision     m_beginOnTime;      //!< The elasped time marker of when the system turned on any active Cooling/Heating
    ::Rte::Element::Precision     m_beginOffTime;     //!< The elasped time marker of when the system turned off all active Cooling/Heating
    ::Rte::Element::Boolean_T     m_systemOn;         //!< Indicates that system is actively Cooling or Heating

public:
    /// Constructor
    TupleSysState( void )
        {
        registerElement( STORM_THERMOSTAT_TUPLE_SYSSTATE_BEGINONTIME,  m_beginOnTime  );
        registerElement( STORM_THERMOSTAT_TUPLE_SYSSTATE_BEGINOFFTIME, m_beginOffTime );
        registerElement( STORM_THERMOSTAT_TUPLE_SYSSTATE_SYSTEMON,     m_systemOn     );
        }              

    /// Resets all of my Pulse Elements
    void resetPulses( void )
        {
        // No Pulses!
        }          

};


/////////////////////////////////////////////////////////////////////
// CONFIGURATION
/////////////////////////////////////////////////////////////////////
#define STORM_THERMOSTAT_TUPLE_CONFIG_HEATINGNUM_PRI_STAGES     0  //!< Element index
#define STORM_THERMOSTAT_TUPLE_CONFIG_HEATINGNUM_SEC_STAGES     1  //!< Element index
#define STORM_THERMOSTAT_TUPLE_CONFIG_NUM_TUPLES                (STORM_THERMOSTAT_TUPLE_CONFIG_HEATINGNUM_SEC_STAGES+1) //!< Element count

/// Tuple
class TupleConfig: public ::Rte::Tuple::Basic<STORM_THERMOSTAT_TUPLE_CONFIG_NUM_TUPLES>
{
public:
    ::Rte::Element::Uinteger8_T     m_heatingNumPriStages;   //!< Heating: Number of enabled PRIMARY heating stages
    ::Rte::Element::Uinteger8_T     m_heatingNumSecStages;   //!< Heating: Number of enabled Secondary heating stages

public:
    /// Constructor
    TupleConfig( void )
        {
        registerElement( STORM_THERMOSTAT_TUPLE_CONFIG_HEATINGNUM_PRI_STAGES, m_heatingNumPriStages  );
        registerElement( STORM_THERMOSTAT_TUPLE_CONFIG_HEATINGNUM_SEC_STAGES, m_heatingNumSecStages );
        }              

    /// Resets all of my Pulse Elements
    void resetPulses( void )
        {
        // No Pulses!
        }          

};


/////////////////////////////////////////////////////////////////////
// SENSORS
/////////////////////////////////////////////////////////////////////
#define STORM_THERMOSTAT_TUPLE_SENSORS_IDT                       0  //!< Element index
#define STORM_THERMOSTAT_TUPLE_SENSORS_NUM_TUPLES                (STORM_THERMOSTAT_TUPLE_SENSORS_IDT+1) //!< Element count

/// Tuple
class TupleSensors: public ::Rte::Tuple::Basic<STORM_THERMOSTAT_TUPLE_SENSORS_NUM_TUPLES>
{
public:
    ::Rte::Element::Float_T     m_idt;                        //!< Indoor temperature

public:
    /// Constructor
    TupleSensors( void )
        {
        registerElement( STORM_THERMOSTAT_TUPLE_SENSORS_IDT, m_idt  );
        }              

    /// Resets all of my Pulse Elements
    void resetPulses( void )
        {
        // No Pulses!
        }          

};

};      // end namespace
};     
#endif  // end header latch
