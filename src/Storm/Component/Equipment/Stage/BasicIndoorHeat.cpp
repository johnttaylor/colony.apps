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


#include "BasicIndoorHeat.h"
#include "Cpl/System/Assert.h"
#include "Storm/Utils/DutyCycle.h"


/// Namespaces
using namespace Storm::Component::Equipment::Stage;



///////////////////////////////
BasicIndoorHeat::BasicIndoorHeat( float pvLowerBound, float pvUpperBound, unsigned comfortStageIndex, unsigned indoorStageIndex, bool controlIndoorFan )
    : Basic( pvLowerBound, pvUpperBound, comfortStageIndex, indoorStageIndex )
    , m_controlFan( controlIndoorFan )
{
    // Initialize my FSM
    initialize();
}

///////////////////////////////
void BasicIndoorHeat::stageOff() noexcept
{
    CPL_SYSTEM_ASSERT( m_args );
    m_args->vOutputs.indoorStages[m_outIndex] = STORM_DM_MP_VIRTUAL_OUTPUTS_OFF;

    // Only change/set the indoor fan when I am the 1st stage
    if ( m_ccIndex == 0 )
    {
        m_args->vOutputs.indoorFan = STORM_DM_MP_VIRTUAL_OUTPUTS_OFF;
    }
}

void BasicIndoorHeat::stageOn() noexcept
{
    CPL_SYSTEM_ASSERT( m_args );
    m_args->vOutputs.indoorStages[m_outIndex] = STORM_DM_MP_VIRTUAL_OUTPUTS_ON;

    // Only change/set the indoor fan when I am the 1st stage
    if ( m_ccIndex == 0 )
    {
        // Turn the indoor fan on if the stage is "electric heat"; else ( aka a 'furnace') turn off the fan and let the furnace controller modulate the indoor fan
        m_args->vOutputs.indoorFan = m_controlFan ? STORM_DM_MP_VIRTUAL_OUTPUTS_ON : STORM_DM_MP_VIRTUAL_OUTPUTS_OFF;
    }
}


///////////////////////////////
uint32_t BasicIndoorHeat::getOffCycleMinTime( Storm::Component::Control::Equipment::Args_T& args ) const noexcept
{
    return args.comfortConfig.heating[m_ccIndex].minOffTime;
}

uint32_t BasicIndoorHeat::getOnCycleMinTime( Storm::Component::Control::Equipment::Args_T& args ) const noexcept
{
    return args.comfortConfig.heating[m_ccIndex].minOnTime;
}

Storm::Type::Cph BasicIndoorHeat::getCycleCph( Storm::Component::Control::Equipment::Args_T& args ) const noexcept
{
    return Storm::Type::Cph::_from_integral_unchecked( args.comfortConfig.heating[m_ccIndex].cph );
}
