#ifndef Storm_Component_PreProcessSensors_h_
#define Storm_Component_PreProcessSensors_h_
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




/// Namespaces
namespace Storm { namespace Component {


/** This mostly concrete class provide the context for a PI controller.  The
    'context' is responsible defining the current delta error input, PI gains
    & reset times, max OUT, etc. to the PI controller. 
 */
class PreProcessSensors: public Base
{
public:
    /// Input Parameters
    struct Input_T
        {
        float                           m_idt;                  //!< Current indoor temperature in degrees Fahrenheit 
        float                           m_ridt;                 //!< Current Remote indoor temperature in degrees Fahrenheit
        bool                            m_haveRemoteIdtSensor;  //!< Indicates that Remote IDT sensor is available
        bool                            m_idtIsValid;           //!< Valid flag:is true when there is valid value/reading for the onboard IDT sensor
        bool                            m_ridtIsValid;          //!< Valid flag:is true when there is valid value/reading for the remote IDT sensor
        };


    /// Output Parameters
    struct Output_T
        {
        float                           m_idt;                  //!< Current indoor temperature in degrees Fahrenheit
        bool                            m_idtIsValid;           //!< Valid flag:is true when there is valid value/reading for IDT 
        };



protected:
    ///Constructor.  
    PreProcessSensors( void );
    

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
