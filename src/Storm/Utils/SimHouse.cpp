/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2019  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

#include "SimHouse.h"

#include <stdio.h>

using namespace Storm::Utils;


////////////////////////////////
SimHouse::SimHouse( double tickPeriodInSeconds,
                    double initialOdt,
                    double maxOdt,
                    double minOdt,
                    double odtCoolingLoadRating,
                    double odtHeatingLoadRating,
                    double systemCoolingEnvResistance,
                    double systemHeatingEnvResistance )
    : m_sim( tickPeriodInSeconds, systemCoolingEnvResistance, maxOdt - minOdt, initialOdt - minOdt )
    , m_maxOdt( maxOdt )
    , m_minOdt( minOdt )
    , m_coolCap( 75.0 - odtCoolingLoadRating )
    , m_heatCap( 68.0 - odtHeatingLoadRating )
    , m_coolingEnvResistance( systemCoolingEnvResistance )
    , m_heatingEnvResistance( systemHeatingEnvResistance )
{
    printf( "coolCap=%g, heatCap=%g\n", m_coolCap, m_heatCap );
}


double SimHouse::tick( double currentOdt, double percentActiveCapacity, bool coolingCapacity ) noexcept
{
    // Limit check the ODT
    if ( currentOdt > m_maxOdt )
    {
        currentOdt = m_maxOdt;
    }
    else if ( currentOdt < m_minOdt )
    {
        currentOdt = m_minOdt;
    }

    // Limit check the capacity
    if ( percentActiveCapacity < 0.0 )
    {
        percentActiveCapacity = 0.0;
    }
    else if ( percentActiveCapacity > 1.0 )
    {
        percentActiveCapacity = 1.0;
    }

    double capacity    = coolingCapacity ? m_coolCap : m_heatCap;
    double resistance  = coolingCapacity ? m_coolingEnvResistance : m_heatingEnvResistance;
    double inPotential = currentOdt - m_minOdt + (capacity * percentActiveCapacity);

    m_sim.start( inPotential, resistance );
    m_sim.accumulate( inPotential, resistance );
    return m_sim.finish() + m_minOdt;
}
