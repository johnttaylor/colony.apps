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
#include "Storm/Type/Pulse.h"


/// Namespaces
namespace Storm { namespace Component {


/** This mostly concrete class implements a simple (P)roportional (I)ntegral 
    controller that only allows the integral term to be a positive number. This 
    class is simplification of a basic PID Controller (see https://en.wikipedia.org/wiki/PID_controller)
    The class has the following 'integral wind-up' protections:

        o Allows many external entities to explicity inhibit the integral term.
          
        o The output of PI is clamped based on the configuration parameter
          m_maxOutValue.  When the output is clamped the integral term is
          inhibited.
          
        o The integral term is clamped such that integral term by itself
          (i.e. with zero error) does not exceed the maximum configured
          output of the PI.
 */
class Pi: public Base
{
public:
    /// Input Parameters
    struct Input_T
        {
        float               m_deltaError;       //!< Delta error between a setpoint (SP) and the process variable (PV).  A positive error increases the output (OUT)
        int32_t             m_freezeRefCount;   //!< Reference counter for external entities to freeze the OUT value (also inhibits the integral term). If the counter is greater than zero, then a new OUT value is calculated.
        int32_t             m_inhibitRefCount;  //!< Reference counter for external entities to inhibit the integral term from changing.  If the counter is greater than zero, then the integral is inhibited
        float               m_gain;             //!< The proportional gain constant for the PI
        float               m_resetTime;        //!< Reset time in milliseconds. The reset time is the time it takes for the intergral term, given a constant error, to effect PI OUT term the same amount as the Proportional gain.
        float               m_maxOutValue;      //!< The maximum allowed OUT value.  The controller will not output a value greater than 'maxOutValue'.  Also the controller ensures that the Integral term, by itself, never exceeds 'maxOutValue'.
        Storm::Type::Pulse  m_reset;            //!< Request to reset the Controller, When this flag is true, the Controller resets its output and integral term to zero.
        };


    /// Output Parameters
    struct Output_T
        {
        float    m_out;                 //!< Output of the PI Controller.  This is unitless positive number that ranges from 0.0 to m_maxOutValue.
        float    m_sumError;            //!< New sum error term
        bool     m_inhibitedState;      //!< This flag is true if the PI Controller's integral is currently inhibited. Note: This flag includes any internal inhibiting of the integral term as well as being set true when the PI 'freezed'
        };


protected:
    // dt interval time - in milliseconds - as a float (instead of Precision_T struct)
    float m_dt;

    // Previous sum error term
    float m_prevSumError;

    // Previous OUT value
    float m_prevOut;


protected:
    ///Constructor.  
    Pi( void );
    

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

};



};      // end namespace
};      // end namespace
#endif  // end header latch
