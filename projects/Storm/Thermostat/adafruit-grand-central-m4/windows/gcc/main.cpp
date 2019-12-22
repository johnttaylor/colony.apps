
#include "Arduino.h"
#include "Bsp/Api.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Storm/Thermostat/Main/Main.h"
#include "Cpl/TShell/Cmd/FreeRTOS/Threads.h"

extern Cpl::Io::InputOutput& Bsp_Serial( void );

static Cpl::TShell::Cmd::FreeRTOS::Threads cmdThreads_( g_cmdlist, "invoke_special_static_constructor" );


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
    // Make the current/main thread a CPL Thread
    Cpl::System::FreeRTOS::Thread::makeNativeMainThreadACplThread();

    // Run the application (Note: This method does not return)
    runTheApplication( Bsp_Serial(), Bsp_Serial() );
}
