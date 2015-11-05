#ifndef Storm_Component_PiContextIdt_h_
#define Storm_Component_PiContextIdt_h_
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
#include "Storm/Type/HeatType.h"
#include "Storm/Type/OMode.h"
#include "Storm/Type/Pulse.h"


/** Magic constant the represent the 'amount' LV per cooling stage
 */
#ifndef OPTION_STORM_COMPONENT_PI_CONTEXT_IDT_COOLING_LV_PER_STAGE
#define OPTION_STORM_COMPONENT_PI_CONTEXT_IDT_COOLING_LV_PER_STAGE     100.0f
#endif


/** Magic constant the represent the 'amount' LV per heating stage
 */
#ifndef OPTION_STORM_COMPONENT_PI_CONTEXT_IDT_HEATING_LV_PER_STAGE
#define OPTION_STORM_COMPONENT_PI_CONTEXT_IDT_HEATING_LV_PER_STAGE     100.0f
#endif


/// Namespaces
namespace Storm { namespace Component {


/** This mostly concrete class provide the context for a PI controller.  The
    'context' is responsible defining the current delta error input, PI gains
    & reset times, max OUT, etc. to the PI controller. 
 */
class PiContextIdt: public Base
{
public:
    /// Input Parameters
    struct Input_T
        {
        unsigned                        m_coolingNumStages;      //!< Cooling: Number of enabled/present cooling stages
        float                           m_coolingGain0;          //!< Cooling: PI gain constant (nominal)
        float                           m_coolingGain1;          //!< Cooling: PI gain constant ('fast')
        float                           m_coolingResetTime0;     //!< Cooling: PI reset time    (nominal)
        float                           m_coolingResetTime1;     //!< Cooling: PI reset time    ('fast')
        float                           m_coolingSetpoint;       //!< Cooling setpoint in degrees Fahrenheit
        unsigned                        m_heatingNumPriStages;   //!< Heating: Number of enabled PRIMARY heating stages
        unsigned                        m_heatingNumSecStages;   //!< Heating: Number of enabled Secondary heating stages
        float                           m_heatingGain0;          //!< Heating: PI gain constant (nominal)
        float                           m_heatingGain1;          //!< Heating: PI gain constant ('fast')
        float                           m_heatingResetTime0;     //!< Heating: PI reset time    (nominal)
        float                           m_heatingResetTime1;     //!< Heating: PI reset time    ('fast')
        float                           m_heatingSetpoint;       //!< Heating setpoint in degrees Fahrenheit
        float                           m_idt;                   //!< Current Indoor temperature
        Storm::Type::OMode::Enum_T      m_opMode;                //!< Actual/Operating mode for the thermostat
        Storm::Type::Pulse              m_opModeChanged;         //!< Indicates that there is/was an operating mode transition
        Storm::Type::HeatType::Enum_T   m_primaryHeatSource;     //!< Heating type of the primary heat source
        Storm::Type::HeatType::Enum_T   m_secondaryHeatSource;   //!< Heating type of the secondary heat source
        bool                            m_noPrimaryHeat;         //!< Use of the Primary heat source has been disabled (aka "Emergency Heat" mode)   
        bool                            m_coolingFastPiEnabled;  //!< Enables the 'fast' cooling PI parameters
        bool                            m_heatingFastPiEnabled;  //!< Enables the 'fast' heating PI parameters
        };


    /// Output Parameters
    struct Output_T
        {
        float               m_deltaError;       //!< Delta error between the ACTIVE setpoint and the indoor temperature. The active setpoint is determined by the current operating mode.
        float               m_deltaSetpoint;    //!< Absolute value of the delta change of the active setpoint.  If m_setpointChanged is false, then this output has NO meaning (i.e. check m_setpointChanged before using this field) 
        Storm::Type::Pulse  m_setpointChanged;  //!< Indicates if the active setpoint changed during THIS processing cycle.  This flag is NOT set on a change of the active setpoint.
        float               m_gain;             //!< The proportional gain constant for the PI
        float               m_resetTime;        //!< PI Reset time in milliseconds.
        float               m_maxOutValue;      //!< The maximum allowed OUT value.  
        };


protected:
    /// Cached active setpoint value;
    float m_prevActiveSetpoint


protected:
    ///Constructor.  
    PiContextIdt( void );
    

protected:
    /// See Storm::Component::Base
    void execute( Cpl::System::ElaspedTime::Precision_T currentTick, 
                  Cpl::System::ElaspedTime::Precision_T currentInterval 
                );



protected:
    /** This method returns the ocnfiguration and run time input paratmers for 
        Component. This method will called at from the context of the execute() 
        method, i.e. every dt interval. The method returns true if successful; 
        else false is return if an error was encounter in retrieving one or more 
        the input parameters. When false is returned the 'errorOccurred' parameter
        of the execute() method is set to true and no additional Component 
        processing is done for the current cycle.
    */
    virtual bool getInputs( Input_T& runtime ) = 0;
  
    /** This method is responsible for publishing/routing/pushing the 
        Component's output upon completion of the current processing cycle. 
        This method will called from the context of the execute() method, i.e. 
        every dt interval. The method returns true if successful; else false is 
        return if an error was encounter in publishing/pushing one or more 
        the output parameters. When false is returned the 'errorOccurred' 
        parameter of the execute() method is set to true.
    */    
    virtual bool setOutputs( Output_T& runtime ) = 0;
     
     

public:
    /// See Storm::Component::Api
    bool start( Cpl::System::ElaspedTime::Precision_T intervalTime );


protected:
    /** Helper method that determines the maximum PI OUT value (aka load value)
        for cooling operation.  The default implementation supports N stages of
        compressor cooling
     */
    virtual float calcPiCoolingMaxOut( Input_T& inputs );

         
    /** Helper method that determines the maximum PI OUT value (aka load value)
        for heating operation.  The default implementation supports multiple 
        stagecompressors as well as multiple stages of Electricl/Fossile heat.  It
        DOES NOT support use cases where the primary and secondary heating
        source can NOT be on at the same time, aka duel fuel scenerios (e.g 
        Heat Pump + furance).
     */
    virtual float calcPiHeatingMaxOut( Input_T& inputs );


};



};      // end namespace
};      // end namespace
#endif  // end header latch
