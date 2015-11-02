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


#include "Pi.h"
#include "Cpl/System/Trace.h"

#define SECT_   "Storm::Component::Pi"


/// Namespaces
using namespace Storm::Component;



///////////////////////////////
Pi::Pi( void )
    {
    }

bool Pi::start( Cpl::System::ElaspedTime::Precision_T intervalTime )
    {
    // Initialize my data
    m_dt           = intervalTime.m_thousandths + intervalTime.m_seconds * 1000;
    m_prevSumError = 0.0f;
    m_prevOut      = 0.0f;

    // Initialize parent class
    return Base::initialize( intervalTime );
    }


///////////////////////////////
bool Pi::execute( Cpl::System::ElaspedTime::Precision_T currentTick, 
                  Cpl::System::ElaspedTime::Precision_T currentInterval
                )
    {
    CPL_SYSTEM_TRACE_FUNC( SECT_ );

    //--------------------------------------------------------------------------
    // Pre-Algorithm processing
    //--------------------------------------------------------------------------

    // Get Inputs
    Input_T inputs;
    if ( !getInputs( &inputs ) )
        {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "[%p] Failed getInputs", this ) );
        return false;
        }

    // Default the output values
    Output_T outputs;
    outputs.m_out            = m_prevOut;
    outputs.m_sumError       = m_prevSumError;
    outputs.m_inhibitedState = false;



    //--------------------------------------------------------------------------
    // Algorithm processing
    //
    // NOTE: Floating point operations that have the potential to overflow the 
    //       precision of a float are up cast to a double then the final result 
    //       is cast back to a float to minimize the arithmetic errors.
    //--------------------------------------------------------------------------

    // Trap a reset-the-Controller request
    if ( inputs.m_reset.isPulsed() )
        {
        outputs.m_out       = m_prevOut       = 0.0f;
        outputs.m_sumError  = m_prevSumError  = 0.0f;
        }

    // Check for freeze-the-output request
    if ( inputs.m_freezeRefCount != 0 )
        {
        outputs.m_inhibitedState = true;
        return true;
        }

    // Sum the delta error (but don't allow negative sums)
    float newSumError = m_prevSumError + inputs.m_deltaError;
    if ( newSumError < 0.0f )
        {
        newSumError = 0.0f;
        }

    // Clamp the sum error when it exceeds the 'max integral' value
    bool  noUpdateToSumError = false;
    float maxSumError        = (float)(( (double)(inputs.m_maxOutValue) * (double)(inputs.m_resetTime) ) / (double)(inputs.m_gain) );
    if ( newSumError > maxSumError )
        {
        newSumError              = maxSumError;
        outputs.m_inhibitedState = true;
        }


    // Calculate the OUT value
    outputs.m_out = (float)( ((double)(inputs.m_gain) * (double)(inputs.m_deltaError) + (double)newSumError * (double)(m_dt)) / (double)(inputs.m_resetTime) )
 

    // Do not let the OUT value go negative
    if ( outputs.m_out < 0.0f )
        {
        outputs.m_out = 0.0f;
        }
    
    // Clamp the OUT value when required
    else if ( outputs.m_out > inputs.m_maxOutValue )
        {
        outputs.m_out      = m_maxOutValue; 
        noUpdateToSumError = true;
        }


    // Update my integral term when not inhibited
    if ( inputs.m_inhibitRefCount == 0 && noUpdateToSumError == false )
        {
        outputs.m_sumError = newSumError;
        }
    else
        {
        outputs.m_inhibitedState = true;
        }

    // Cache my final outputs
    m_prevSumError = outputs.m_sumError;
    m_prevOut      = outputs.m_out;



    //--------------------------------------------------------------------------
    // Post-Algorithm processing
    //--------------------------------------------------------------------------

    // All done -->publish the outputs
    if ( !setOutputs( &outputs ) )
        {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "[%p] Failed setOutputs", this ) );
        return false;
        }

    // If I get here -->everything worked!
    return true;
    }


