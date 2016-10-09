#ifndef Storm_Component_PreProcessConfig_h_
#define Storm_Component_PreProcessConfig_h_
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
#include "Storm/Type/Element/HeatType.h"




/// Namespaces
namespace Storm { namespace Component {


/** This mostly concrete class provide the context for a PI controller.  The
    'context' is responsible defining the current delta error input, PI gains
    & reset times, max OUT, etc. to the PI controller. 
 */
class PreProcessConfig: public Base
{
public:
    /// Input Parameters
    struct Input_T
        {
        Storm::Type::HeatType::Enum_T   m_primaryHeatSource;     //!< Heating type of the primary heat source
        Storm::Type::HeatType::Enum_T   m_secondaryHeatSource;   //!< Heating type of the secondary heat source
        unsigned                        m_numCompHeatingStages;  //!< Number of compressor heating stages (0-N)
        unsigned                        m_numElecHeatingStages;  //!< Number of Electric heating stages (0-N)
        unsigned                        m_numFossilHeatingStages;//!< Number of Furnace heating stages (0-N)
        };


    /// Output Parameters
    struct Output_T
        {
        unsigned                        m_heatingNumPriStages;   //!< Heating: Number of enabled PRIMARY heating stages
        unsigned                        m_heatingNumSecStages;   //!< Heating: Number of enabled Secondary heating stages
        };


protected:
    ///Constructor.  
    PreProcessConfig( void );
    

protected:
    /// See Storm::Component::Base
    bool execute( Cpl::System::ElaspedTime::Precision_T currentTick, 
                  Cpl::System::ElaspedTime::Precision_T currentInterval 
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
    bool start( Cpl::System::ElaspedTime::Precision_T intervalTime );


protected:
    /** Helper method */
    virtual unsigned getNumHeatStages( Storm::Type::HeatType::Enum_T source, Input_T& inputs );

};



};      // end namespace
};      // end namespace
#endif  // end header latch
