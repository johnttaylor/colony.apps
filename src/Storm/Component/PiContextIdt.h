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

#include "Storm/Component/Base.h"
#include "Cpl/Type/SeqNumber.h"


/// Namespaces
namespace Storm { namespace Component {


/** This mostly concrete class provide the context for a PI controller.  The
    'context' is responsible defining the current delta error input to the
    PI controller. 

           
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
class PiContextIdt: public Base
{
public:
    /// Configuration Parameters.
    struct Configuration_T
        {
        float  m_coolingSetpoint;               //!< Cooling setpoint in degrees Fahrenheit
        float  m_heatingSetpoint;               //!< Heating setpoint in degrees Fahrenheit
        };

    /// Input Parameters
    struct Input_T
        {
        float                      m_idt;           //!< Current Indoor temperature
        Storm::Type::OMode::Enum_t m_opMode;        //!< Actual/Operating mode for the thermostat
        Storm::Type::Pulse         m_opModeChanged; //!< Indicates that there is/was an operating mode transition
        };


    /// Output Parameters
    struct Output_T
        {
        float               m_deltaError;       //!< Delta error between the ACTIVE setpoint (SP) and the process variable (PV). The active setpoint is determined by the current operating mode.
        float               m_deltaSetpoint;    //!< Absolute value of the delta change of the active setpoint.  If m_setpointChanged is false, then this output has NO meaning (i.e. check m_setpointChanged before using this field) 
        Storm::Type::Pulse  m_setpointChanged;  //!< Indicates if the active setpoint changed during THIS processing cycle.  This flag is NOT set on a change of the active setpoint.
        };


protected:
    /// Cached active setpoint value;
    float m_activeSetpoint


protected:
    ///Constructor.  
    PiContextIdt( void );
    

protected:
    /// See Storm::Component::Base
    void execute( Cpl::System::ElaspedTime::Precision_T currentTick, 
                  Cpl::System::ElaspedTime::Precision_T currentInterval 
                );


protected:
    /// Hook in to the base implementation (See Storm::Component::Base)
    void setInterval( Cpl::System::ElaspedTime::Precision_T newInterval );
    

protected:
    /** This method returns the configuration/turning parameters for the PI
        controller.  This method will called at from the context of the execute()
        method, i.e. every dt interval. The method returns true if successful; 
        else false is return if an error was encounter in retrieving one or more 
        the configuration parameters.
     */
    virtual bool getConfiguration( Configuration_T& cfg ) = 0;

   
   /** This method returns the run time input paratmers for the PI Controller. 
       This method will called at from the context of the execute() method, 
       i.e. every dt interval. The method returns true if successful; else false 
       is return if an error was encounter in retrieving one or more 
       the input parameters.
    */
    virtual bool getInputs( Input_T& inputs ) = 0;
  
   /** This method provide the PI Controller's output to be processed/routed
       by the concrete child class. This method will called at from the context 
       of the execute() method, i.e. every dt interval. The method returns true 
       if successful; else false is return if an error was encounter in 
       'outputting' at least one of the output parameters.  When false is return
       the PI controller will internally inhibit the integral term until this
       method returns true.
    */    
    virtual bool setOutputs( Output_T& outputs ) = 0;
     
     

protected:
    /// Helper method to called from start()
    void initialize( void );

};



};      // end namespace
};      // end namespace
#endif  // end header latch
