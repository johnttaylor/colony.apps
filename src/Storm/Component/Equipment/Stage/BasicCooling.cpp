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


#include "BasicCooling.h"
#include "Cpl/System/Assert.h"


/// Namespaces
using namespace Storm::Component::Equipment::Stage;



///////////////////////////////
BasicCooling::BasicCooling( float pvLowerBound, float pvUpperBound, unsigned comfortStageIndex, unsigned outdoorStageIndex, unsigned stageIndex )
    : Basic( pvLowerBound, pvUpperBound )
    , m_ccIndex( comfortStageIndex )
    , m_outIndex( outdoorStageIndex )
    , m_stageIndex( stageIndex )
{
    // Initialize my FSM
    initialize();
}

void BasicCooling::configure( float pvLowerBound, float pvUpperBound, unsigned comfortStageIndex, unsigned outdoorStageIndex, unsigned stageIndex )
{
    m_pvLowerBound = pvLowerBound;
    m_pvUpperBound = pvUpperBound;
    m_ccIndex      = comfortStageIndex;
    m_outIndex     = outdoorStageIndex;
    m_stageIndex   = stageIndex;
}

///////////////////////////////

void BasicCooling::initializeActive() noexcept
{
    CPL_SYSTEM_ASSERT( m_args );

    Basic::initializeActive();
    m_args->vOutputs.sovInHeating = false;
}

void BasicCooling::stageOff() noexcept
{
    CPL_SYSTEM_ASSERT( m_args );
    m_args->vOutputs.outdoorStages[m_outIndex] = STORM_DM_MP_VIRTUAL_OUTPUTS_OFF;
    
    // Only true the indoor fan off when the 1st stage is turned off
    if ( m_stageIndex == 0 )
    {
        m_args->vOutputs.indoorFan = STORM_DM_MP_VIRTUAL_OUTPUTS_OFF;
    }
}

void BasicCooling::stageOn() noexcept
{
    CPL_SYSTEM_ASSERT( m_args );
    m_args->vOutputs.outdoorStages[m_outIndex] = STORM_DM_MP_VIRTUAL_OUTPUTS_ON;
    
    // Only true the indoor fan off on the 1st stage is turned on
    if ( m_stageIndex == 0 )
    {
        m_args->vOutputs.indoorFan = STORM_DM_MP_VIRTUAL_OUTPUTS_ON;
    }
}

uint32_t BasicCooling::getOffCycleMinTime( Storm::Component::Control::Equipment::Args_T& args ) const noexcept
{
    return args.comfortConfig.cooling[m_ccIndex].minOffTime;
}

uint32_t BasicCooling::getOnCycleMinTime( Storm::Component::Control::Equipment::Args_T& args ) const noexcept
{
    return args.comfortConfig.cooling[m_ccIndex].minOnTime;
}

Storm::Type::Cph BasicCooling::getCycleCph( Storm::Component::Control::Equipment::Args_T& args ) const noexcept
{
    return Storm::Type::Cph::_from_integral_unchecked( args.comfortConfig.cooling[m_ccIndex].cph );
}


