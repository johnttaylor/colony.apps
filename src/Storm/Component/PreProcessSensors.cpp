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


#include "PreProcessSensors.h"
#include "Cpl/System/Trace.h"


#define SECT_               "Storm::Component::PreProcessSensors"




/// Namespaces
using namespace Storm::Component;


///////////////////////////////
PreProcessSensors::PreProcessSensors( void )
    {
    }


bool PreProcessSensors::start( Cpl::System::ElaspedTime::Precision_T intervalTime )
    {
    // Initialize parent class
    return Base::initialize( intervalTime );
    }


///////////////////////////////
bool PreProcessSensors::execute( Cpl::System::ElaspedTime::Precision_T currentTick, 
                                 Cpl::System::ElaspedTime::Precision_T currentInterval 
                               )
    {
    CPL_SYSTEM_TRACE_FUNC( SECT_ );

    //--------------------------------------------------------------------------
    // Pre-Algorithm processing
    //--------------------------------------------------------------------------

    // Get Inputs
    Input_T  inputs;
    if ( !getInputs( &inputs ) )
        {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "[%p] Failed getInputs", this ) );
        return false;
        }

    // Default the output values
    Output_T outputs;
    outputs.m_idt       = inputs.m_idt;
    outputs.m_idt_valid = inputs.m_idtIsValid;


    //--------------------------------------------------------------------------
    // Algorithm processing
    //--------------------------------------------------------------------------

    if ( inputs.m_haveRemoteIdtSensor && inputs.m_ridtIsValid )
        {
        outputs.m_idt        = inputs.m_ridt;
        outputs.m_idtIsValid = inputs.m_ridtIsValid;
        }


    //--------------------------------------------------------------------------
    // Post-Algorithm processing
    //--------------------------------------------------------------------------

    // All done -->set the outputs
    if ( !setOutputs( &outputs ) )
        {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "[%p] Failed setOutputs", this ) );
        return false;
        }

    return true;
    }




