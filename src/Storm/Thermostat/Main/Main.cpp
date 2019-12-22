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

#include "colony_config.h"
#include "Main.h"
#include "Private_.h"
#include "Cpl/System/Shutdown.h"
#include "Storm/Thermostat/Algorithm.h"
#include "Storm/Thermostat/ModelPoints.h"
#include "Storm/TShell/State.h"
#include "Cpl/TShell/Cmd/Help.h"
#include "Cpl/TShell/Cmd/Bye.h"
#include "Cpl/TShell/Cmd/Trace.h"
#include "Cpl/TShell/Cmd/TPrint.h"
#include "Cpl/Dm/TShell/Dm.h"
#include "Cpl/TShell/Maker.h"
#include "Cpl/Dm/MailboxServer.h"
#include "Cpl/System/Thread.h"
#include "Cpl/TShell/Stdio.h"
#include "Cpl/System/EventLoop.h"
#include "Cpl/System/Semaphore.h"



////////////////////////////////////////////////////////////////////////////////
Cpl::Container::Map<Cpl::TShell::Command>           g_cmdlist( "ignore_this_parameter-used to invoke the static constructor" );
static Cpl::TShell::Maker                           cmdProcessor_( g_cmdlist );
static Cpl::TShell::Stdio                           shell_( cmdProcessor_, "TShell", CPL_SYSTEM_THREAD_PRIORITY_NORMAL + CPL_SYSTEM_THREAD_PRIORITY_LOWER + CPL_SYSTEM_THREAD_PRIORITY_LOWER );
static Cpl::TShell::Cmd::Help	                    helpCmd_( g_cmdlist, "invoke_special_static_constructor" );
static Cpl::TShell::Cmd::Bye	                    byeCmd_( g_cmdlist, "invoke_special_static_constructor" );
static Cpl::TShell::Cmd::Trace	                    traceCmd_( g_cmdlist, "invoke_special_static_constructor" );
static Cpl::TShell::Cmd::TPrint	                    tprintCmd_( g_cmdlist, "invoke_special_static_constructor" );
static Cpl::Dm::TShell::Dm	                        dmCmd_( g_cmdlist, g_modelDatabase, "invoke_special_static_constructor", "dm" );
static Storm::TShell::State	                        stateCmd_( g_cmdlist, "invoke_special_static_constructor" );

static Storm::Thermostat::Algorithm thermostatAlgorithm_;

static Cpl::System::Semaphore       waitForShutdown_;
static volatile int                 exitCode_;

static void initializeModelPoints() noexcept;
static int runShutdownHandlers() noexcept;

// Allocate/create my Model Database
// NOTE: For the MickySoft compiler I must 'allocate' the g_modelDatabase before any
//       model points get instantiated.  By placing the allocation in the Main 
//       directory AND by using nqbp's 'firstObjects' feature (and setting the Main
//       directory to be a 'firstObjects') it seems to appease the MS gods.
Cpl::Dm::ModelDatabase   g_modelDatabase( "ignoreThisParameter_usedToInvokeTheStaticConstructor" );

int runTheApplication( Cpl::Io::Input& infd, Cpl::Io::Output& outfd )
{
    // Start the shell
    shell_.launch( infd, outfd );

    // Create thread to run the Algorithm
    Cpl::System::Thread::create( thermostatAlgorithm_, "Algorithm", CPL_SYSTEM_THREAD_PRIORITY_NORMAL + CPL_SYSTEM_THREAD_PRIORITY_RAISE );


    // Start the algorithm
    initializeModelPoints();
    thermostatAlgorithm_.open();

    // Wait for the Application to be shutdown
    waitForShutdown_.wait();

    // Shutdown application objects
    thermostatAlgorithm_.close();

    // Run any/all register shutdown handlers (as registered by the Cpl::System::Shutdown interface) and then exit
    return runShutdownHandlers();
}


#define SETPOINT_COOLING        77.0F
#define SETPOINT_HEATING        70.0F
#define INITIAL_PRIMARY_IDT     75.0F
#define INITIAL_SECONDARY_IDT   71.0F

void initializeModelPoints() noexcept
{
    mp_setpoints.write( SETPOINT_COOLING, SETPOINT_HEATING );
    mp_userMode.write( Storm::Type::ThermostatMode::eOFF );
    mp_fanMode.write( Storm::Type::FanMode::eAUTO );
    mp_primaryRawIdt.write( INITIAL_PRIMARY_IDT );
    mp_secondaryRawIdt.write( INITIAL_SECONDARY_IDT );
    mp_activeIdt.setInvalid();
    mp_relayOutputs.setSafeAllOff();
    mp_idtAlarms.setAlarm( false, false, false );
    mp_noActiveConditioningAlarm.setAlarm( false, false );
    mp_userCfgModeAlarm.setAlarm( false, false );
    mp_enabledSecondaryIdt.write( false );
    mp_equipmentConfig.writeCompressorStages( 1 );
    mp_equipmentConfig.writeIndoorFanMotor( false );
    mp_equipmentConfig.writeIndoorHeatingStages( 1 );
    mp_equipmentConfig.writeIndoorType( Storm::Type::IduType::eFURNACE );
    mp_equipmentConfig.writeOutdoorType( Storm::Type::OduType::eAC );
    Storm::Type::ComfortStageParameters_T configConfig = { Storm::Type::Cph::e3CPH, 5 * 60, 5 * 50 };
    mp_comfortConfig.writeCompressorCooling( configConfig );
    mp_comfortConfig.writeCompressorHeating( configConfig );
    mp_comfortConfig.writeIndoorHeating( configConfig );
    mp_systemForcedOffRefCnt.reset();
    mp_systemConfig.setInvalid();           // Algorithm will update this!
    mp_systemOn.write( false );
    mp_resetPiPulse.write( false );
    mp_operatingModeChanged.write( false );
    mp_deltaIdtError.write( 0.0F );
    mp_deltaSetpoint.write( 0.0F );
    mp_setpointChanged.write( false );
    mp_activeSetpoint.setInvalid();         // Algorithm will update this!    
    mp_freezePiRefCnt.reset();
    mp_inhibitfRefCnt.reset();
    mp_pvOut.write( 0.0F );
    mp_sumError.write( 0.0F );
    mp_pvInhibited.write( false );
    Storm::Type::VirtualOutputs_T zeroVOutputs = { 0, };
    mp_vOutputs.write( zeroVOutputs );
    Storm::Type::CycleInfo_T zeroCycleInfo                = { 0, };
    mp_cycleInfo.write( zeroCycleInfo );
    Storm::Type::EquipmentTimes_T zeroEquipmentBeginTimes = { 0, };
    mp_equipmentBeginTimes.write( zeroEquipmentBeginTimes );
}



////////////////////////////////////////////////////////////////////////////////
int runShutdownHandlers() noexcept
{
    exitCode_ = exitPlatform( Cpl::System::Shutdown::notifyShutdownHandlers_( exitCode_ ) );
    return exitPlatform( exitCode_ );
}

int Cpl::System::Shutdown::success( void )
{
    exitCode_ = OPTION_CPL_SYSTEM_SHUTDOWN_SUCCESS_ERROR_CODE;
    waitForShutdown_.signal();
    return OPTION_CPL_SYSTEM_SHUTDOWN_SUCCESS_ERROR_CODE;
}

int Cpl::System::Shutdown::failure( int exit_code )
{
    exitCode_ = exit_code;
    waitForShutdown_.signal();
    return exit_code;
}