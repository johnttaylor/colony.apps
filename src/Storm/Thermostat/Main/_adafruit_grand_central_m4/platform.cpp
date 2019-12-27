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

#include "Storm/Thermostat/Main/Main.h"
#include "Storm/Thermostat/Main/Private_.h"
#include "Storm/Thermostat/SimHouse/Cmd.h"
#include "Storm/Thermostat/SimHouse/House.h"
#include "Storm/Thermostat/Logger.h"
#include "Storm/Thermostat/Outputs.h"
#include "Storm/Thermostat/ModelPoints.h"
#include "Cpl/TShell/Cmd/FreeRTOS/Threads.h"
#include "Cpl/System/Thread.h"
#include "task.h"
#include <Adafruit_NeoPixel.h>

static Cpl::TShell::Cmd::FreeRTOS::Threads  cmdThreads_( g_cmdlist, "invoke_special_static_constructor" );
static Storm::Thermostat::SimHouse::Cmd     houseCmd_( g_cmdlist, "invoke_special_static_constructor" );

static Storm::Thermostat::SimHouse::House   houseSimulator_;

// Get a reference to the NeoPixel driver
extern Adafruit_NeoPixel g_pixels;


////////////////////////////////////////////////////////////////////////////////

void initializePlatform0()
{
    // Create thread to run the House simulation
    Cpl::System::Thread::create( houseSimulator_, "HouseSim", CPL_SYSTEM_THREAD_PRIORITY_NORMAL );
}

void openPlatform0()
{
}

void closePlatform0()
{
}

int exitPlatform( int exitCode )
{
    vTaskEndScheduler();
    return exitCode;
}

void Storm::Thermostat::Logger::recordSystemData()
{
    // No logging support (yet...)
}


////////////////////////////////////////////////////////////////////////
#define HAS_CHANGED()   (prevSeqNumRelays != seqNumRelays || prevSeqNumNoActiveCond != seqNumNoActiveCond || prevSeqNumUserCfgMode != seqNumUserCfgMode || prevSeqNumIdtAlarms != seqNumIdtAlarms )
#define SET_PREVIOUS()  prevSeqNumRelays = seqNumRelays; prevSeqNumNoActiveCond = seqNumNoActiveCond; prevSeqNumUserCfgMode = seqNumUserCfgMode; prevSeqNumIdtAlarms = seqNumIdtAlarms 

#define G_PIXEL              1
#define BK_PIXEL             2
#define W1_PIXEL             3
#define O_PIXEL              4
#define Y1_PIXEL             5
#define IDT_ALARM_PIXEL      6
#define USER_CFG_ALARM_PIXEL 7
#define NO_COND_ALARM_PIXEL  8
#define NEXT_PIXEL_ROW       8

#define PIXEL_ON             64
#define PIXEL_OFF            0

void Storm::Thermostat::Outputs::updateHVACOutputs()
{
    static uint16_t prevSeqNumRelays       = Cpl::Dm::ModelPoint::SEQUENCE_NUMBER_UNKNOWN;
    static uint16_t prevSeqNumNoActiveCond = Cpl::Dm::ModelPoint::SEQUENCE_NUMBER_UNKNOWN;
    static uint16_t prevSeqNumUserCfgMode  = Cpl::Dm::ModelPoint::SEQUENCE_NUMBER_UNKNOWN;
    static uint16_t prevSeqNumIdtAlarms    = Cpl::Dm::ModelPoint::SEQUENCE_NUMBER_UNKNOWN;

    // Get Output data
    Storm::Type::HvacRelayOutputs_T relays;
    Storm::Dm::MpSimpleAlarm::Data  noActiveCond;
    Storm::Dm::MpSimpleAlarm::Data  userCfgMode;
    Storm::Dm::MpIdtAlarm::Data     idt;
    uint16_t                        seqNumRelays;
    uint16_t                        seqNumNoActiveCond;
    uint16_t                        seqNumUserCfgMode;
    uint16_t                        seqNumIdtAlarms;
    if ( Cpl::Dm::ModelPoint::IS_VALID( mp_relayOutputs.read( relays, &seqNumRelays ) ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( mp_noActiveConditioningAlarm.read( noActiveCond, &seqNumNoActiveCond ) ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( mp_userCfgModeAlarm.read( userCfgMode, &seqNumUserCfgMode ) ) == false ||
         Cpl::Dm::ModelPoint::IS_VALID( mp_idtAlarms.read( idt, &seqNumIdtAlarms ) ) == false )
    {
        if ( HAS_CHANGED() )
        {
            // Turn everything off
            g_pixels.begin();
            for ( int i=0; i < BUILD_OPT_NUM_PIXELS; i++ )
            {
                g_pixels.setPixelColor( i, g_pixels.Color( 0, 0, 0, 0 ) );
            }
            g_pixels.show();
        }

        SET_PREVIOUS();
        return;
    }

    // LED Mapping:
    // G: Pixel  #1,  Green
    // BK: Pixel #2,  Green >0%
    // BK: Pixel #10, Green >25%
    // BK: Pixel #18, Green >50%
    // BK: Pixel #22, Green >75%
    // W1: Pixel #3,  White
    // W2: Pixel #11, White
    // W3: Pixel #19, White
    // O:  Pixel #4,  Cooling: Blue, Heating: Red
    // Y1: Pixel #5,  Yellow
    // Y2: Pixel #13, Yellow
    // IDT Alarm, Pixel #6,  Primary IDT Fault,   yellow=non-critical, red=critical
    // IDT Alarm, Pixel #15, Secondary IDT Fault, yellow=non-critical, red=critical
    // User Cfg Mode Alarm,          Pixel #7, yellow=non-critical, red-critical
    // No Active Conditioning Alarm, Pixel #8, yellow=non-critical, red-critical
    if ( HAS_CHANGED() )
    {
        g_pixels.begin();

        g_pixels.setPixelColor( G_PIXEL, g_pixels.Color( 0, relays.g ? PIXEL_ON : PIXEL_OFF, 0, 0 ) );

        g_pixels.setPixelColor( BK_PIXEL, g_pixels.Color( 0, relays.bk > 0 ? PIXEL_ON : PIXEL_OFF, 0, 0 ) );
        g_pixels.setPixelColor( BK_PIXEL + NEXT_PIXEL_ROW, g_pixels.Color( 0, relays.bk > 25 ? PIXEL_ON : PIXEL_OFF, 0, 0 ) );
        g_pixels.setPixelColor( BK_PIXEL + NEXT_PIXEL_ROW + NEXT_PIXEL_ROW, g_pixels.Color( 0, relays.bk > 50 ? PIXEL_ON : PIXEL_OFF, 0, 0 ) );
        g_pixels.setPixelColor( BK_PIXEL + NEXT_PIXEL_ROW + NEXT_PIXEL_ROW + NEXT_PIXEL_ROW, g_pixels.Color( 0, relays.bk > 75 ? PIXEL_ON : PIXEL_OFF, 0, 0 ) );

        g_pixels.setPixelColor( W1_PIXEL, g_pixels.Color( 0, 0, 0, relays.w1 ? PIXEL_ON : PIXEL_OFF ) );
        g_pixels.setPixelColor( W1_PIXEL + NEXT_PIXEL_ROW, g_pixels.Color( 0, 0, 0, relays.w2 ? PIXEL_ON : PIXEL_OFF ) );
        g_pixels.setPixelColor( W1_PIXEL + NEXT_PIXEL_ROW + NEXT_PIXEL_ROW, g_pixels.Color( 0, 0, 0, relays.w3 ? PIXEL_ON : PIXEL_OFF ) );

        g_pixels.setPixelColor( O_PIXEL, g_pixels.Color( relays.o ? PIXEL_OFF : PIXEL_ON, 0, relays.o ? PIXEL_ON : PIXEL_OFF, 0 ) );

        g_pixels.setPixelColor( Y1_PIXEL, g_pixels.Color( relays.y1 ? PIXEL_ON : PIXEL_OFF, relays.y1 ? PIXEL_ON : PIXEL_OFF, 0, 0 ) );
        g_pixels.setPixelColor( Y1_PIXEL + NEXT_PIXEL_ROW, g_pixels.Color( relays.y2 ? PIXEL_ON : PIXEL_OFF, relays.y2 ? PIXEL_ON : PIXEL_OFF, 0, 0 ) );

        if ( idt.critical )
        {
            g_pixels.setPixelColor( IDT_ALARM_PIXEL, g_pixels.Color( idt.primaryAlarm ? PIXEL_ON : PIXEL_OFF, 0, 0, 0 ) );
            g_pixels.setPixelColor( IDT_ALARM_PIXEL + NEXT_PIXEL_ROW, g_pixels.Color( idt.secondaryAlarm ? PIXEL_ON : PIXEL_OFF, 0, 0, 0 ) );
        }
        else
        {
            g_pixels.setPixelColor( IDT_ALARM_PIXEL, g_pixels.Color( idt.primaryAlarm ? PIXEL_ON : PIXEL_OFF, idt.primaryAlarm ? PIXEL_ON : PIXEL_OFF, 0, 0 ) );
            g_pixels.setPixelColor( IDT_ALARM_PIXEL + NEXT_PIXEL_ROW, g_pixels.Color( idt.secondaryAlarm ? PIXEL_ON : PIXEL_OFF, idt.secondaryAlarm ? PIXEL_ON : PIXEL_OFF, 0, 0 ) );
        }

        if ( userCfgMode.critical )
        {
            g_pixels.setPixelColor( USER_CFG_ALARM_PIXEL, g_pixels.Color( userCfgMode.active ? PIXEL_ON : PIXEL_OFF, 0, 0, 0 ) );
        }
        else
        {
            g_pixels.setPixelColor( USER_CFG_ALARM_PIXEL, g_pixels.Color( userCfgMode.active ? PIXEL_ON : PIXEL_OFF, userCfgMode.active ? PIXEL_ON : PIXEL_OFF, 0, 0 ) );
        }

        if ( noActiveCond.critical )
        {
            g_pixels.setPixelColor( NO_COND_ALARM_PIXEL, g_pixels.Color( noActiveCond.active ? PIXEL_ON : PIXEL_OFF, 0, 0, 0 ) );
        }
        else
        {
            g_pixels.setPixelColor( NO_COND_ALARM_PIXEL, g_pixels.Color( noActiveCond.active ? PIXEL_ON : PIXEL_OFF, noActiveCond.active ? PIXEL_ON : PIXEL_OFF, 0, 0 ) );
        }

        g_pixels.show();
    }

    SET_PREVIOUS();
}

