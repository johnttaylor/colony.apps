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

void Pi::initialize( void )
    {
    m_cachedSeqNum.invalidate();  // Set to an invalid value so first processing cycle will clear the integral
    m_dt = 0.0;
    }

void Pi::setInterval( Cpl::System::ElaspedTime::Precision_T newInterval )
    {
    Base::setInterval( newInterval );
    m_dt = newInterval.m_thousandths + newInterval.m_seconds * 1000;
    }


///////////////////////////////
void Pi::execute( Cpl::System::ElaspedTime::Precision_T currentTick, 
                  Cpl::System::ElaspedTime::Precision_T currentInterval 
                )
    {
    CPL_SYSTEM_TRACE_FUNC( SECT_ );

    // NOTE: Floating point operations that have the potential to overflow the 
    //       precision of a float are up cast to a double then the final result 
    //       is cast back to a float to minimize the arithmetic errors.
   

    // Get Config & Inputs
    Configuration_T cfg;
    Input_T         inputs;
    if ( !getConfiguration( &cfg ) )
        {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "[%p] Failed getConfiguration", this ) );
        return;
        }
    if ( !getInputs( &inputs ) )
        {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "[%p] Failed getInputs", this ) );
        return;
        }

    // Trap a reset-the-Controller request
    if ( inputs.m_reset != m_cachedReset )
        {
        m_cachedReset         = inputs.m_reset;
        inputs.m_prevSumError = 0.0f;
        inputs.m_prevOut      = 0.0f;
        }

    // Default the output values
    Output_T outputs;
    outputs.m_newOut         = inputs.m_prevOut;
    outputs.m_newSumError    = inputs.m_prevSumError;
    outputs.m_inhibitedState = false;


    // Check for freeze-the-output request
    if ( inputs.m_freezeRefCount != 0 )
        {
        outputs.m_inhibitedState = true;
        return;
        }

    // Sum the delta error (but don't allow negative sums)
    float newSumError = inputs.m_prevSumError + inputs.m_deltaError;
    if ( newSumError < 0.0f )
        {
        newSumError = 0.0f;
        }

    // Clamp the sum error when it exceeds the 'max integral' value
    bool  noUpdateToSumError = false;
    float maxSumError        = (float)(( (double)(cfg.m_maxOutValue) * (double)(cfg.m_resetTime) ) / (double)(cfg.m_gain) );
    if ( newSumError > maxSumError )
        {
        newSumError              = maxSumError;
        outputs.m_inhibitedState = true;
        }


    // Calculate the OUT value
    outputs.m_newOut = (float)( ((double)(cfg.m_gain) * (double)(inputs.m_deltaError) + (double)newSumError * (double)(m_dt)) / (double)(cfg.m_resetTime) )
 

    // Do not let the OUT value go negative
    if ( outputs.m_newOut < 0.0f )
        {
        outputs.m_newOut = 0.0f;
        }
    
    // Clamp the OUT value when required
    else if ( outputs.m_newOut > cfg.m_maxOutValue )
        {
        outputs.m_newOut   = m_maxOutValue; 
        noUpdateToSumError = true;
        }


    // Update my integral term when not inhibited
    if ( inputs.m_inhibitRefCount == 0 && noUpdateToSumError == false )
        {
        outputs.m_newSumError = newSumError;
        }
    else
        {
        outputs.m_inhibitedState = true;
        }


    // All done -->set the outputs
    if ( !setOutputs( &outputs ) )
        {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "[%p] Failed setOutputs", this ) );
        }
    }


