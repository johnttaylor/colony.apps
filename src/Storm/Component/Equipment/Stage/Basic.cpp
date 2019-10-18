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


#include "Basic.h"
#include "Cpl/System/Assert.h"


/// Namespaces
using namespace Storm::Component::Equipment::Stage;



///////////////////////////////
Basic::Basic( void )
    : m_args( 0 )
    , m_nextStage( 0 )
    , m_prevStage( 0 )
    , m_startInOffCycle( false )
    , m_supplemented( false )
{
}

///////////////////////////////
void Basic::requestOn( Storm::Component::Control::Equipment::Args_T& args, bool startInOnCycle ) noexcept
{
    m_args            = &args;
    m_startInOffCycle = !startInOnCycle;
    m_supplemented    = false;
    generateEvent( Fsm_evOnRequest );
}

void Basic::requestAsSupplement( Storm::Component::Control::Equipment::Args_T& args, Stage& nextStage, bool startNextStageInOnCycle ) noexcept
{
    m_args            = &args;
    m_startInOffCycle = !startNextStageInOnCycle;
    m_nextStage       = &nextStage;
    generateEvent( Fsm_evNeedMoreCapacity );
}

void Basic::requestModeToOff( Storm::Component::Control::Equipment::Args_T& args ) noexcept
{
    generateEvent( Fsm_evOffModeRequest );
}

void Basic::requestOff( Storm::Component::Control::Equipment::Args_T& args, bool startLowerStageInOnCycle ) noexcept
{
    m_args            = &args;
    m_startInOffCycle = !startLowerStageInOnCycle;
    generateEvent( Fsm_evExcessCapacity );
}

void Basic::execute( Storm::Component::Control::Equipment::Args_T& args ) noexcept
{
    m_args = &args;
    generateEvent( FSM_NO_MSG );
}

void Basic::notifyAsActiveStage_( Storm::Component::Control::Equipment::Args_T& args, Stage& previousStage, bool startInOnCycle ) noexcept
{
    m_args            = &args;
    m_startInOffCycle = !startInOnCycle;
    m_supplemented    = true;
    m_nextStage       = 0;
    m_prevStage       = &previousStage;
    generateEvent( Fsm_evOnRequest );
}

void Basic::notifyAsExitingSupplmenting_( Storm::Component::Control::Equipment::Args_T& args, bool startInOnCycle ) noexcept
{
    m_args            = &args;
    m_startInOffCycle = !startInOnCycle;
    m_nextStage       = 0;
    generateEvent( Fsm_evLessCapacityNeeded );
}


///////////////////////////////
void Basic::initializeStage() noexcept
{
    m_startInOffCycle = false;
    m_supplemented    = false;
    m_nextStage       = 0;
    m_prevStage       = 0;
    m_args            = 0;
    shutdownStage();
}

void Basic::enterSupplementing() noexcept
{
    CPL_SYSTEM_ASSERT( m_args );
    CPL_SYSTEM_ASSERT( m_nextStage );

    m_nextStage->notifyAsActiveStage_( *m_args, *this, !m_startInOffCycle );
}

void Basic::exitSupplementing() noexcept
{

}

void Basic::notifyLowerStage() noexcept
{
    CPL_SYSTEM_ASSERT( m_args );
    CPL_SYSTEM_ASSERT( m_prevStage );

    m_prevStage->notifyAsExitingSupplmenting_( *m_args, !m_startInOffCycle );
}

///////////////////////////////
bool Basic::isStartInOffCycle() noexcept
{
    return m_startInOffCycle;
}

bool Basic::isBeingSupplemented() noexcept
{
    return m_supplemented;
}
