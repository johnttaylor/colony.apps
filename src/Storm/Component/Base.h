#ifndef Storm_Component_Base_h_
#define Storm_Component_Base_h_
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


#include "Storm/Component/Api.h"


/// Namespaces
namespace Storm { namespace Component {


/** This partially concrete class provide core infrastructure for Components.
 */
class Base: public Api
{
protected:
    /// The component's initial periodic time interval for execution
    Cpl::System::ElaspedTime::Precision_T   m_interval;

    /// The last time the Component executed
    Cpl::System::ElaspedTime::Precision_T   m_timeMark;

    /// Slippage counter
    unsigned long                           m_slipCounter;

    /// State of the last interval time maker
    bool                                    m_timeMarkValid;

    /// The internal started/stopped state
    bool                                    m_started;


public:               
     */
    Base( void );


public:
    /** This method handles the time management for a Component. The execute() 
        method below is called as part of this method -- this means that 
        Components typicall do not implement and/or override this method -> they 
        simply implement the execute() method.
     */
    bool do( bool enabled, Cpl::System::ElaspedTime::Precision_T currentTick );

    
protected:
    /** This method is for/when the component to execute it logic.  This method
        will ONLY be called when component's periodic time/interval has elasped.
        Components that have the same periodic time/interval will all be 
        synchonized to the same tick/boundary.

        The 'currentTick' value represents the current time of the main() loop
        interation, i.e. the time of the start of the loop iteration.  The 
        'currentInterval' is the deterministic interval boundary/time of when
        the component is logically executing.  For example: if a module has
        an execution interval of 100ms and the iteration of the main() loop
        started at 13.212 seconds then:
        
            currentTick:=       { 13s, 212ms }
            currentInterval:=   { 13s, 200ms }

        The method is required to return true when succesfully; else return
        false when an error occurred.

        NOTE: If the 'enabled' argument in the do() method (which calls this
              method) is false, then this method is NOT called.

     */
    virtual bool execute( Cpl::System::ElaspedTime::Precision_T currentTick, 
                          Cpl::System::ElaspedTime::Precision_T currentInterval
                        ) = 0;


protected:
    /** A concrete component calls this method to set its periodic execution
        rate/interval. This method MUST be called from the concrete 
        implementation of the start() method. The concrete implementation is
        also RESPONSIBLE for calling this method when/if the Application changes
        the component's interval timing. The 'interval' argument is the periodic 
        time interval (in milliseconds) between component executions, e.g. a 
        value of '50' means the component's execute() method will be called/run at 20Hz.
     */
    virtual void setInterval( Cpl::System::ElaspedTime::Precision_T newInterval );


    /** This method is called when a Component has detected that it not executing
        as fast as it designed/configured execution interval.  The default action
        does NOTHING.
     */
    virtual boid manageSlippage( Cpl::System::ElaspedTime::Precision_T currentTick );
};


};      // end namespace
};      
#endif  // end header latch
