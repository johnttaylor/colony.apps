#ifndef Storm_Component_IdtSelection_h_
#define Storm_Component_IdtSelection_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Apps Project.  The Colony.Apps Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.apps/license.txt
*
* Copyright (c) 2015 - 2019 John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

#include "colony_config.h"
#include "Storm/Component/Base.h"
#include "Cpl/Dm/Mp/Float.h"
#include "Cpl/Dm/Mp/Bool.h"
#include "Cpl/Dm/Mp/RefCounter.h"




/// Namespaces
namespace Storm {
/// Namespaces
namespace Component {


/** This concrete class determine which Indoor-Temperature (IDT) value to be used
    for the rest of the algorithm.  In addition, it is responsible for generating
    alarms associated with the IDT inputs.
 */
class IdtSelection : public Base
{
public:
    /// Input Model Points
    struct Input_T
    {
        Cpl::Dm::Mp::Float&		primaryIdt;                 //!< Current indoor temperature in degrees Fahrenheit of the PRIMARY IDT sensor (aka the onboard sensor)
        Cpl::Dm::Mp::Float&		secondaryIdt;               //!< Current indoor temperature in degrees Fahrenheit of the SECONDARY IDT sensor (aka the remote sensor)
        Cpl::Dm::Mp::Bool&		haveSecondaryIdt;			//!< Indicates that Secondary IDT sensor is available
        Cpl::Dm::Mp::RefCounter systemForcedOffRefCntl		//!< Reference Counter: When greater the zero the system is required to be forced off.
    };


    /// Output Parameters
    struct Output_T
    {
        Cpl::Dm::Mp::Float&     activeIdt;					//!< Current indoor temperature in degrees Fahrenheit to be used for the algorithm
        Cpl::Dm::Mp::RefCounter systemForcedOffRefCntl;		//!< Reference Counter: When greater the zero the system is required to be forced off.
    };



protected:
    ///Constructor.  
    IdtSelection( void );


protected:
    /// See Storm::Component::Base
    bool execute( Cpl::System::ElapsedTime::Precision_T currentTick,
        Cpl::System::ElapsedTime::Precision_T currentInterval
    );



protected:
    /** This method returns the ocnfiguration and run time input paratmers for
        Component. This method will be called from the context of the execute()
        method, i.e. every dt interval. The method returns true if successful;
        else false is return if an error was encounter in retrieving one or more
        the input parameters. When false is returned the 'errorOccurred' parameter
        of the execute() method is set to true and no additional Component
        processing is done for the current cycle.
    */
    virtual bool getInputs( Input_T& runtime ) = 0;

    /** This method is responsible for publishing/routing/pushing the
        Component's output upon completion of the current processing cycle.
        This method will be called from the context of the execute() method, i.e.
        every dt interval. The method returns true if successful; else false is
        return if an error was encounter in publishing/pushing one or more
        the output parameters. When false is returned the 'errorOccurred'
        parameter of the execute() method is set to true.
    */
    virtual bool setOutputs( Output_T& runtime ) = 0;



public:
    /// See Storm::Component::Api
    bool start( Cpl::System::ElapsedTime::Precision_T intervalTime );


};



};      // end namespace
};      // end namespace
#endif  // end header latch
