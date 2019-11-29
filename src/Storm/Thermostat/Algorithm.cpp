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


#include "Algorithm.h"
#include "ModelPoints.h"
#include "Cpl/System/FatalError.h"

#define ALGORITHM_PROCESSING_INTERVAL_SEC      2

/// Namespace
using namespace Storm::Thermostat;



static Storm::Component::IdtSelection::Input_T  idtSelection_ins_          = { mp_primaryRawIdt , mp_secondaryRawIdt, mp_enabledSecondaryIdt };
static Storm::Component::IdtSelection::Output_T idtSelection_outs_         = { mp_activeIdt, mp_systemForcedOffRefCnt , mp_idtAlarms };
       
static Storm::Component::OperatingMode::Input_T  operatingMode_ins_        = { mp_setpoints,  mp_userMode, mp_activeIdt, mp_equipmentBeginTimes, mp_systemOn, mp_systemForcedOffRefCnt, mp_equipmentConfig, mp_comfortConfig };
static Storm::Component::OperatingMode::Output_T operatingMode_outs_       = { mp_operatingModeChanged, mp_resetPiPulse, mp_systemForcedOffRefCnt, mp_systemConfig, mp_noActiveConditioningAlarm, mp_userCfgModeAlarm };
       
static Storm::Component::PiPreProcess::Input_T  piPreProcess_ins_          = { mp_activeIdt, mp_systemConfig, mp_operatingModeChanged, mp_setpoints };
static Storm::Component::PiPreProcess::Output_T piPreProcess_outs_         = { mp_activeSetpoint, mp_deltaIdtError, mp_deltaSetpoint, mp_setpointChanged };
       
static Storm::Component::Pi::Input_T  pi_ins_                              = { mp_resetPiPulse, mp_deltaIdtError, mp_systemConfig, mp_freezePiRefCnt, mp_inhibitfRefCnt };
static Storm::Component::Pi::Output_T pi_outs_                             = { mp_pvOut, mp_sumError, mp_pvInhibited };
       
static Storm::Component::Control::Input_T  control_ins_                    = { mp_systemConfig, mp_pvOut, mp_vOutputs, mp_equipmentBeginTimes, mp_systemOn, mp_cycleInfo };
static Storm::Component::Control::Output_T control_outs_                   = { mp_vOutputs, mp_cycleInfo, mp_systemOn };
       
static Storm::Component::FanControl::Input_T  fanControl_ins_              = { mp_fanMode,  mp_systemConfig, mp_vOutputs, mp_equipmentBeginTimes };
static Storm::Component::FanControl::Output_T fanControl_outs_             = { mp_vOutputs };
       
static Storm::Component::HvacRelayOutputs::Input_T  hvavRelayOutputs_ins_  = { mp_vOutputs, mp_equipmentBeginTimes, mp_systemForcedOffRefCnt, mp_systemOn };
static Storm::Component::HvacRelayOutputs::Output_T hvavRelayOutputs_outs_ = { mp_equipmentBeginTimes, mp_relayOutputs };


///////////////////////////////
Algorithm::Algorithm()
    : Cpl::Dm::MailboxServer( OPTION_CPL_SYSTEM_EVENT_LOOP_TIMEOUT_PERIOD )
    , Cpl::Itc::CloseSync( *((Cpl::Itc::PostApi*)this) )
    , Cpl::System::Timer( *((Cpl::System::TimerManager*)this) )
    , m_idtSelection( idtSelection_ins_, idtSelection_outs_ )
    , m_operatingMode( operatingMode_ins_, operatingMode_outs_ )
    , m_piPreProcess( piPreProcess_ins_, piPreProcess_outs_ )
    , m_pi( pi_ins_, pi_outs_ )
    , m_controlCooling( m_equipmentCooling, control_ins_, control_outs_ )
    , m_controlIdHeating( m_equipmentIndoorHeating, control_ins_, control_outs_ )
    , m_controlOff( m_equipmentOff, control_ins_, control_outs_ )
    , m_fanControl( fanControl_ins_, fanControl_outs_ )
    , m_hvacRelayOutputs( hvavRelayOutputs_ins_, hvavRelayOutputs_outs_ )
    , m_equipmentCooling( m_stage1Cooling )
    , m_equipmentIndoorHeating( m_stage1IndoorHeat, m_stage2IndoorHeat, m_stage3IndoorHeat )
    , m_equipmentOff()
    , m_stage1Cooling( 0, 0 )
    , m_stage1IndoorHeat( 0, 0 )
    , m_stage2IndoorHeat( 1, 1 )
    , m_stage3IndoorHeat( 2, 2 )
    , m_opened( false )
{
}




///////////////////////////////
void Algorithm::startComponents( void ) noexcept
{
    Cpl::System::ElapsedTime::Precision_T time = { ALGORITHM_PROCESSING_INTERVAL_SEC, 0 };

    m_idtSelection.start( time );
    m_operatingMode.start( time );
    m_piPreProcess.start( time );
    m_pi.start( time );
    m_controlCooling.start( time );
    m_controlIdHeating.start( time );
    m_controlOff.start( time );
    m_fanControl.start( time );
    m_hvacRelayOutputs.start( time );
}


void Algorithm::expired( void ) noexcept
{
    // Restart my interval timer
    Cpl::System::ElapsedTime::Precision_T time = Cpl::System::ElapsedTime::precision();
    Timer::start( ALGORITHM_PROCESSING_INTERVAL_SEC  * 1000 );

    // Reset all Pulse MPs
    mp_resetPiPulse.write( false );

    // Execute the algorithm
    bool success = true;
    success &= m_idtSelection.doWork( success, time );
    success &= m_operatingMode.doWork( success, time );
    success &= m_piPreProcess.doWork( success, time );
    success &= m_pi.doWork( success, time );
    success &= m_controlCooling.doWork( success, time );
    success &= m_controlIdHeating.doWork( success, time );
    success &= m_controlOff.doWork( success, time );
    success &= m_fanControl.doWork( success, time );
    success &= m_hvacRelayOutputs.doWork( success, time );

    // Something broke!!  Not sure what make the most sense here -->for now throw a fatal error
    if ( !success )
    {
        Cpl::System::FatalError::logf( "Algorithm::executeAlgorithm: One or more Components failed! (time=%lu.%u", time.m_seconds, time.m_thousandths );
    }
}

///////////////////////////////
void Algorithm::request( Cpl::Itc::OpenRequest::OpenMsg& msg )
{
    if ( !m_opened )
    {
        m_opened = true;
        Timer::start( ALGORITHM_PROCESSING_INTERVAL_SEC * 1000 );
        startComponents();
    }

    msg.returnToSender();
}

void Algorithm::request( Cpl::Itc::CloseRequest::CloseMsg& msg )
{
    if ( m_opened )
    {
        m_opened = false;
        Timer::stop(); 
    }

    msg.returnToSender();
}



