
#include "Arduino.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Bsp/Api.h"
#include "Cpl/Io/InputOutput.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Cpl/Container/Map.h"
#include "Cpl/TShell/Maker.h"
#include "Cpl/TShell/Stdio.h"
#include "Cpl/TShell/Cmd/Help.h"
#include "Cpl/TShell/Cmd/Bye.h"
#include "Cpl/TShell/Cmd/Trace.h"
#include "Cpl/TShell/Cmd/TPrint.h"
#include "Cpl/TShell/Cmd/FreeRTOS/Threads.h"


extern Cpl::Io::InputOutput& Bsp_Serial( void );


static Cpl::Container::Map<Cpl::TShell::Command>    cmdlist_;
static Cpl::TShell::Maker                           cmdProcessor_( cmdlist_ );
static Cpl::TShell::Stdio                           shell_( cmdProcessor_, "TShell" );

static Cpl::TShell::Cmd::Help	                    helpCmd_( cmdlist_, "invoke_special_static_constructor" );
static Cpl::TShell::Cmd::Bye	                    byeCmd_( cmdlist_, "invoke_special_static_constructor" );
static Cpl::TShell::Cmd::Trace	                    traceCmd_( cmdlist_, "invoke_special_static_constructor" );
static Cpl::TShell::Cmd::TPrint	                    tprintCmd_( cmdlist_, "invoke_special_static_constructor" );
static Cpl::TShell::Cmd::FreeRTOS::Threads          threads_( cmdlist_, "invoke_special_static_constructor" );

// the setup function runs once when you press reset or power the board
void setup()
{
    // Initialize the board (for use with CPL)
    Bsp_Api_initialize();
    Bsp_beginArduinoSerialObject( 115200, SERIAL_8N1 );

    // Initialize CPL
    Cpl::System::Api::initialize();
}


// the loop function runs over and over again forever
void loop()
{
    static bool testsHaveBeenStarted = false;
    if ( !testsHaveBeenStarted )
    {
        testsHaveBeenStarted = true;

        // Make the current/main thread a CPL Thread
        Cpl::System::FreeRTOS::Thread::makeNativeMainThreadACplThread();

        // Start the shell
        shell_.launch( Bsp_Serial(), Bsp_Serial() );
    }

    // Blink the LED!!!
    digitalWrite( LED_BUILTIN, HIGH );      
    vTaskDelay( 1000 );                     
    digitalWrite( LED_BUILTIN, LOW );       
    vTaskDelay( 500 );                      

    digitalWrite( LED_BUILTIN, HIGH );      
    vTaskDelay( 400 );                      
    digitalWrite( LED_BUILTIN, LOW );       
    vTaskDelay( 300 );                      
    digitalWrite( LED_BUILTIN, HIGH );      
    vTaskDelay( 200 );                      
    digitalWrite( LED_BUILTIN, LOW );       
    vTaskDelay( 100 );                      
}
