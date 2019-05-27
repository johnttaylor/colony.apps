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


#include "Pi.h"
#include "Cpl/System/Trace.h"

#define SECT_   "Storm::Component::Pi"


/// Namespaces
using namespace Storm::Component;



///////////////////////////////
Pi::Pi( Rte::Element::Float&          i_deltaError,
        Rte::Element::Integer32&      i_freezeRefCount,
        Rte::Element::Integer32&      i_inhibitRefCount,
        Rte::Element::Float&          i_gain,
        Rte::Element::Float&          i_resetTime,
        Rte::Element::Float&          i_maxOutValue,
        Storm::Type::Element::Pulse&  i_reset,
        Rte::Element::Float&          o_out,
        Rte::Element::Float&          o_sumError,
        Rte::Element::Boolean&        o_inhibitedState
)
    : mi_deltaError( i_deltaError )
    , mi_freezeRefCount( i_freezeRefCount )
    , mi_inhibitRefCount( i_inhibitRefCount )
    , mi_gain( i_gain )
    , mi_resetTime( i_resetTime )
    , mi_maxOutValue( i_maxOutValue )
    , mi_reset( i_reset )
    , mo_out( o_out )
    , mo_sumError( o_sumError )
    , mo_inhibitedState( o_inhibitedState )
    {
    }

bool Pi::start( Cpl::System::ElapsedTime::Precision_T intervalTime )
    {
    // Initialize my data
    m_dt           = (float)intervalTime.m_thousandths + intervalTime.m_seconds * 1000;
    m_prevSumError = 0.0f;
    m_prevOut      = 0.0f;

    // Initialize parent class
    return Base::start( intervalTime );
    }


///////////////////////////////
bool Pi::execute( Cpl::System::ElapsedTime::Precision_T currentTick,
                  Cpl::System::ElapsedTime::Precision_T currentInterval
                )
    {
    CPL_SYSTEM_TRACE_FUNC( SECT_ );

    //--------------------------------------------------------------------------
    // Pre-Algorithm processing
    //--------------------------------------------------------------------------

    // Default the output values
    mo_out.set( m_prevOut );
    mo_sumError.set( m_prevSumError );
    mo_inhibitedState.set( false );



    //--------------------------------------------------------------------------
    // Algorithm processing
    //
    // NOTE: Floating point operations that have the potential to overflow the 
    //       precision of a float are up cast to a double then the final result 
    //       is cast back to a float to minimize the arithmetic errors.
    //--------------------------------------------------------------------------

    // Trap a reset-the-Controller request
    if (mi_reset.isPulsed())
        {
        mo_out.set( m_prevOut = 0.0f );
        mo_sumError.set( m_prevSumError = 0.0f );
        }

    // Check for freeze-the-output request
    if (mi_freezeRefCount.get() != 0)
        {
        mo_inhibitedState.set( true );
        }

    // NOT stopped/frozen -->go do stuff :)
    else
        {
        // Sum the delta error (but don't allow negative sums)
        float newSumError = m_prevSumError + mi_deltaError.get();
        if (newSumError < 0.0f)
            {
            newSumError = 0.0f;
            }

        // Clamp the sum error when it exceeds the 'max integral' value
        bool  noUpdateToSumError = false;
        float maxSumError        = (float)(((double)(mi_maxOutValue.get()) * (double)(mi_resetTime.get())) / (double)(mi_gain.get()));
        if (newSumError > maxSumError)
            {
            newSumError = maxSumError;
            mo_inhibitedState.set( true );
            }


        // Calculate the OUT value
        mo_out.set( (float)((double)(mi_gain.get()) * ((double)(mi_deltaError.get()) + (((double)newSumError * (double)(m_dt)) / (double)(mi_resetTime.get())))) );


        // Do not let the OUT value go negative
        if (mo_out.get() < 0.0f)
            {
            mo_out.set( 0.0f );
            }

        // Clamp the OUT value when required
        else if (mo_out.get() > mi_maxOutValue.get())
            {
            mo_out.set( mi_maxOutValue.get() );
            noUpdateToSumError = true;
            }


        // Update my integral term when not inhibited
        if (mi_inhibitRefCount.get() == 0 && noUpdateToSumError == false)
            {
            mo_sumError.set( newSumError );
            }
        else
            {
            mo_inhibitedState.set( true );
            }

        // Cache my final outputs
        m_prevSumError = mo_sumError.get();
        m_prevOut      = mo_out.get();
        }


    // If I get here -->everything worked!
    return true;
    }


