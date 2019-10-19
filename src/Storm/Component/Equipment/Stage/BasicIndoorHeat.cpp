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
    : m_pvLowerBound( pvLowerBound )
    , m_pvUpperBound( pvUpperBound )
    , m_ccIndex( comfortStageIndex )
    , m_outIndex( indoorStageIndex )
    , m_controlFan( controlIndoorFan )
{
    // Initialize my FSM
    initialize();
}

void BasicIndoorHeat::configure( float pvLowerBound, float pvUpperBound, unsigned comfortStageIndex, unsigned indoorStageIndex, bool controlIndoorFan )
{
    m_pvLowerBound = pvLowerBound;
    m_pvUpperBound = pvUpperBound;
    m_ccIndex      = comfortStageIndex;
    m_outIndex     = indoorStageIndex;
    m_controlFan   = controlIndoorFan;
}

///////////////////////////////
void BasicIndoorHeat::checkBackTransition() noexcept
{
    // Do nothing -->this action should never be called because we immediately transition to the Off state
}

void BasicIndoorHeat::checkFromTransition() noexcept
{
    // Do nothing -->this action should never be called because we immediately transition to the Cycle state
}

void BasicIndoorHeat::checkOffTime() noexcept
{
    CPL_SYSTEM_ASSERT( m_args );

    // Calculate the CURRENT Off cycle time
    m_args->cycleInfo.onTime = Storm::Utils::DutyCycle::calculateOffTime( m_args->pvOut,
                                                                          m_args->comfortConfig.heating[m_ccIndex].minOffTime,
                                                                          Storm::Type::Cph::_from_integral_unchecked( m_args->comfortConfig.heating[m_ccIndex].cph ),
                                                                          m_pvLowerBound,
                                                                          m_pvUpperBound );

    // Has the off time cycle expired?
    Cpl::System::ElapsedTime::Precision_T cycleTime;
    cycleTime = m_args->cycleInfo.onTime;
    if ( Cpl::System::ElapsedTime::expiredPrecision( m_startTime, cycleTime, m_args->currentInterval ) )
    {
        generateEvent( Fsm_evOffTimeExpired );
    }
}

void BasicIndoorHeat::checkOnTime() noexcept
{
    CPL_SYSTEM_ASSERT( m_args );

    // Calculate the CURRENT On cycle time
    m_args->cycleInfo.onTime = Storm::Utils::DutyCycle::calculateOnTime( m_args->pvOut,
                                                                         m_args->comfortConfig.heating[m_ccIndex].minOnTime,
                                                                         Storm::Type::Cph::_from_integral_unchecked( m_args->comfortConfig.heating[m_ccIndex].cph ),
                                                                         m_pvLowerBound,
                                                                         m_pvUpperBound );

    // Has the on time cycle expired?
    Cpl::System::ElapsedTime::Precision_T cycleTime;
    cycleTime = m_args->cycleInfo.onTime;
    if ( Cpl::System::ElapsedTime::expiredPrecision( m_startTime, cycleTime, m_args->currentInterval ) )
    {
        generateEvent( Fsm_evOnTimeExpired );
    }
}

void BasicIndoorHeat::checkStartingOffTime() noexcept
{
    // Do nothing -->this action should never be called because we immediately transition to the OffTime state
}

void BasicIndoorHeat::checkStartingOnTime() noexcept
{
    // Do nothing -->this action should never be called because we immediately transition to the OnTime state
}

void BasicIndoorHeat::enterSupplementing() noexcept
{
    // Nothing 'extra' needed (over the parent method)
    Basic::enterSupplementing();
}

void BasicIndoorHeat::exitSupplementing() noexcept
{
    // Nothing action needed/required
}

void BasicIndoorHeat::initializeActive() noexcept
{
    CPL_SYSTEM_ASSERT( m_args );
    stageOff();
}

void BasicIndoorHeat::initializeBackTransition() noexcept
{
    // Perform an immediate transition
    generateEvent( Fsm_evBackTransitionCompleted );
}


void BasicIndoorHeat::initializeFromTransition() noexcept
{
    // Perform an immediate transition
    generateEvent( Fsm_evFromTransitionCompleted );
}


void BasicIndoorHeat::shutdownStage() noexcept
{
    stageOff();
}

void BasicIndoorHeat::stageOff() noexcept
{
    CPL_SYSTEM_ASSERT( m_args );
    m_args->vOutputs.outdoorStages[m_outIndex] = STORM_DM_MP_VIRTUAL_OUTPUTS_OFF;
    if ( m_controlFan )
    {
        m_args->vOutputs.indoorFan = STORM_DM_MP_VIRTUAL_OUTPUTS_OFF;
    }
}

void BasicIndoorHeat::stageOn() noexcept
{
    CPL_SYSTEM_ASSERT( m_args );
    m_args->vOutputs.outdoorStages[m_outIndex] = STORM_DM_MP_VIRTUAL_OUTPUTS_ON;
    if ( m_controlFan )
    {
        m_args->vOutputs.indoorFan = STORM_DM_MP_VIRTUAL_OUTPUTS_ON;
    }

}

void BasicIndoorHeat::startCyclingInOffCycle() noexcept
{
    // Nothing action needed/required
}

void BasicIndoorHeat::startCyclingInOnCycle() noexcept
{
    // Nothing action needed/required
}

void BasicIndoorHeat::startingStageOff() noexcept
{
    CPL_SYSTEM_ASSERT( m_args );

    // Turn off the stage and Update the cycle info's starting time
    stageOff();
    m_args->cycleInfo.beginOffTime = m_startTime;
    m_args->cycleInfo.mode         = Storm::Type::CycleStatus::eOFF_CYCLE;

    // Calculate (and check) the current OFF cycle time
    checkOffTime();

    // Transition directly to the OffTime state (i.e. no fan-off delays supported)
    generateEvent( Fsm_evStartingOffTimeExpired );
}

void BasicIndoorHeat::startingStageOn() noexcept
{
    CPL_SYSTEM_ASSERT( m_args );

    // Turn on the stage and Update the cycle info's starting time
    stageOn();
    m_args->cycleInfo.beginOnTime = m_startTime;
    m_args->cycleInfo.mode        = Storm::Type::CycleStatus::eON_CYCLE;

    // Calculate (and check) the current ON cycle time
    checkOnTime();

    // Transition directly to the OnTime state (i.e. no fan-on delays supported)
    generateEvent( Fsm_evStartingOnTimeExpired );
}
