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


#include "DutyCycle.h"
#include "Cpl/Math/real.h"
#include "Cpl/System/FatalError.h"

using namespace Storm::Component;

static float k_[DutyCycle::eNUM_OPTIONS] =
{
    /* 2CPH @ 50% = 15min  */ 15.0F * 60 * 1000 * 0.5F,
    /* 3CPH @ 50% = 10min  */ 10.0F * 60 * 1000 * 0.5F,
    /* 4CPH @ 50% = 7.5min */ 7.5F * 60 * 1000 * 0.5F,
    /* 5CPH @ 50% = 6min   */ 6.0F * 60 * 1000 * 0.5F,
    /* 6CPH @ 50% = 5min   */ 5.0F * 60 * 1000 * 0.5F
};

///////////////////////////////

static float normalizePv( float pvVar, float pvLowerBound, float pvUpperBound )
{
    // Normalize the Process Variable (to a percentage) for the specified range
    float range = pvUpperBound - pvLowerBound;
    if ( pvVar < pvLowerBound )
    {
        pvVar = 0.0F;
    }
    else
    {
        pvVar = ( pvVar - pvLowerBound ) / range;
        if ( pvVar > 1.0F )
        {
            pvVar = 1.0F;
        }
    }

    return pvVar;
}

uint32_t DutyCycle::calculateOffTime( float pvVar, uint32_t minOffTime, Cph_T cyclePerHour, float pvLowerBound, float pvUpperBound )
{
    // Trap invalid CPH option
    if ( cyclePerHour == eNUM_OPTIONS )
    {
        Cpl::System::FatalError::logf( "DutyCycle::calculateOffTime: Invalid CPH selection (pvVar=%g, minOffTime=%d, lower=%g, upper=%g)", pvVar, minOffTime, pvLowerBound, pvUpperBound );
    }

    // Normalize the Process Variable (to a percentage) for the specified range
    pvVar = normalizePv( pvVar, pvLowerBound, pvUpperBound );

    // Calc off time
    uint32_t cycleTime = ( uint32_t) ( k_[cyclePerHour] * ( 1.0F - pvVar ) );

    // Enforce min off time
    if ( cycleTime < minOffTime )
    {
        cycleTime = minOffTime;
    }

    return cycleTime;
}

uint32_t DutyCycle::calculateOnTime( float pvVar, uint32_t minOnTime, Cph_T cyclePerHour, float pvLowerBound, float pvUpperBound )
{
    // Trap invalid CPH option
    if ( cyclePerHour == eNUM_OPTIONS )
    {
        Cpl::System::FatalError::logf( "DutyCycle::calculateOnTime: Invalid CPH selection (pvVar=%g, minOffTime=%d, lower=%g, upper=%g)", pvVar, minOnTime, pvLowerBound, pvUpperBound );
    }

    // Normalize the Process Variable (to a percentage) for the specified range
    pvVar = normalizePv( pvVar, pvLowerBound, pvUpperBound );

    // Calc on time
    uint32_t cycleTime = ( uint32_t) ( k_[cyclePerHour] * pvVar );

    // Enforce min on time
    if ( cycleTime < minOnTime )
    {
        cycleTime = minOnTime;
    }

    return cycleTime;
}


