#ifndef Storm_Component_Pi_h_
#define Storm_Component_Pi_h_
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


/** This mostly concrete class implements a simple (P)roportional (I)ntegral 
    controller that only allows the integral term to be a positive number. This 
    class is simplification of a basic PID Controller (see https://en.wikipedia.org/wiki/PID_controller)
    The class has the following 'integral wind-up' protections:

        o Allows many external entities to explicity inhibit the integral term.
          
        o The output of PI is clamped (based on the configuration parameter
          m_maxOutValue).  When the output is clamped the integral term is
          inhibited.
          
        o The integral term is clamped such that integral term by itself
          (i.e. with zero error) does not exceed the maximum configured
          output of the PI.
          
           
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
class Pi: public Base
{
public:
    /// Configuration Parameters.
    struct Configuration_T
        {
        float    m_gain;                //!< The multipler/gain applied to the Proportional term 
        float    m_resetTime;           //!< Reset time in milliseconds. The reset time is the time it takes for the intergral term, given a constant error, to effect PI OUT term the same amount as the Proportional gain.
        float    m_maxOutValue;         //!< The maximum allowed OUT value.  The controller will not output a value greater than 'maxOutValue'.  Also the controller ensures that the Integral term, by itself, never exceeds 'maxOutValue'.
        };

    /// Input Parameters
    struct Input_T
        {
        float                   m_prevOut;          //!< Previous Output of the PI Controller.  This is unitless positive number that ranges from 0.0 to m_maxOutValue.
        float                   m_prevSumError;     //!< Previous sum error term. Tis the integrated (over dt) delta error value, i.e. the core component of the integral term.
        float                   m_deltaError;       //!< Delta error between a setpoint (SP) and the process variable (PV).  A positive error increases the output (OUT)
        int32_t                 m_freezeRefCount;   //!< Reference counter for external entities to freeze the OUT value (also inhibits the integral term). If the counter is greater than zero, then a new OUT value is calculated.
        int32_t                 m_inhibitRefCount;  //!< Reference counter for external entities to inhibit the integral term from changing.  If the counter is greater than zero, then the integral is inhibited
        Cpl::Type::SeqNumber    m_reset;            //!< Sequence number used to reset the Controller, When the Controller instance see a change the sequence number value, it resets its output and integral term to zero and then updates is internal value for the sequence number to inputted value. 
        };


    /// Output Parameters
    struct Output_T
        {
        float    m_newOut;              //!< Output of the PI Controller.  This is unitless positive number that ranges from 0.0 to m_maxOutValue.
        float    m_newSumError;         //!< New sum error term
        bool     m_inhibitedState;      //!< This flag is true if the PI Controller's integral is currently inhibited.
        };


protected:
    /// Cached 'reset' sequence number
    int32_t m_cachedReset;

    // dt interval time - in milliseconds - as a float (instead of Precision_T struct)
    float   m_dt;


protected:
    ///Constructor.  
    Pi( void );
    

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
