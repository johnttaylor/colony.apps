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

#include "Storm/Component/Base.h"
#include "Storm/Type/TMode.h"
#include "Storm/Type/OMode.h"
#include "Storm/Type/SystemState.h"
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
namespace Storm { namespace Component {


/** This mostly concrete class determine the actual mode of operation for the
    thermostat when the user has selected "Auto Mode".  It is also responsible
    for determine the 'active' setpoint and calculation the delta error value
    for use by the PI Component.
           

    NOTES: 
    
        o A final concrete child class is required to provide configuration,
          inputs, and manage the outputs.  In addition, the child class is
          responsible for implementing the start() and stop() methods of the
          Component API.
          
        o The child class MUST configure the dt interval timing of the instance 
          in its the start() method.

        o The child class MUST call the initialize() method in its the start() 
          method.
 */
class OperatingMode: public Base
{
public:
    /// Configuration Parameters.  
    struct Configuration_T
        {
        float                                   m_coolingSetpoint;  //!< Cooling setpoint in degrees Fahrenheit
        float                                   m_heatingSetpoint;  //!< Heating setpoint in degrees Fahrenheit
        Storm::Type::TMode::Enum_T              m_userMode;         //!< The thermostat mode to be resolved
        };


    /// Runtime Input Parameters
    struct Input_T
        {
        float                                   m_idt;              //!< The current indoor temperature in degrees Fahrenheit
        int32_t                                 m_freezePiRefCount; //!< Current/Pass-through freeze-the-PI-controller reference counter
        Cpl::System::ElaspedTime::Precision_T   m_beginOffTime;     //!< The elasped time marker of when the system turned off all active Cooling/Heating
        Storm::Type::Pulse                      m_resetPi;          //!< Current/Pass-through reset-the-PI-controller request
        bool                                    m_systemOn;         //!< Indicates that system is actively Cooling or Heating
        };


    /// Runtime Output Parameters
    struct Output_T
        {
        int32_t                                 m_freezePiRefCount; //!< Potentially new freeze-the-PI-controller reference counter (when the operating mode transitions to off, the algorithm will freeze the PI controller and reset the controller; then unfreezes on a transition to non-off mode)
        Storm::Type::OMode::Enum_T              m_opMode;           //!< Actual/Operating mode for the thermostat
        Storm::Type::Pulse                      m_resetPi;          //!< Potentially new reset-the-PI-controller request (on a mode change this class will reset the PI component)
        Storm::Type::Pulse                      m_opModeChanged;    //!< Indicates that there is/was an operating mode transition
        float                                   m_deltaError;       //!< Delta error between the ACTIVE setpoint and the Indoor Temperature. The active setpoint is determined by the current operating mode.
        float                                   m_activeSetpoint;   //!< The active setpoint in degrees Fahrenheit.
        float                                   m_deltaSetpoint;    //!< Absolute value of the delta change of the active setpoint.  If m_setpointChanged is false, then this output has NO meaning (i.e. check m_setpointChanged before using this field) 
        Storm::Type::Pulse                      m_setpointChanged;  //!< Indicates if the active setpoint changed during THIS processing cycle.  This flag is NOT set when source of the the active setpoint changes, i.e. NOT set when there is operating mode change.
        };


protected:
    /// Cached active setpoint value;
    float                       m_prevActiveSetpoint;

    /// Current/Previous operating mode
    Storm::Type::OMode::Enum_T  m_prevOperatingMode;

    /// Flag used to detect the transition to AUTO mode
    bool                        m_inAuto;


protected:
    ///Constructor.  
    OperatingMode( void );
    

protected:
    /// See Storm::Component::Base
    bool execute( Cpl::System::ElaspedTime::Precision_T currentTick, 
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
    virtual bool getInputs( Configuration_T& cfg, Input_T& runtime ) = 0;
  
    /** This method is responsible for publishing/routing/pushing the 
        Component's output upon completion of the current processing cycle. 
        This method will called from the context of the execute() method, i.e. 
        every dt interval. The method returns true if successful; else false is 
        return if an error was encounter in publishing/pushing one or more 
        the output parameters. When false is returned the 'errorOccurred' 
        parameter of the execute() method is set to true.
    */    
    virtual bool setOutputs( Output_T& runtime ) = 0;
     

protected:
    /// Helper method to be called from start() (by the concrete child class)
    void initialize( void );

    /// Helper method
    void setNewOMode( Ouput_T& outputs, Storm::Type::OMode::Enum_T newOMode, float newActiveSetpoint, float direction );


};



};      // end namespace
};      // end namespace
#endif  // end header latch
