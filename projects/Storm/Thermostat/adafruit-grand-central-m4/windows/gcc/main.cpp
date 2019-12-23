
#include "Arduino.h"
#include "Bsp/Api.h"
#include "Cpl/System/Api.h"
#include "Cpl/System/FreeRTOS/Thread.h"
#include "Storm/Thermostat/Main/Main.h"
#include "Cpl/Io/InputOutput.h"
#include <Adafruit_NeoPixel.h>

extern Cpl::Io::InputOutput& Bsp_Serial( void );

// NeoPixel Configuration: (Values supplied by the build script!)
#define PIN            BUILD_OPT_PIN        
#define NUMPIXELS      BUILD_OPT_NUM_PIXELS 
#define NEO_TYPE       BUILD_OPT_NEO_TYPE   

Adafruit_NeoPixel pixels_ = Adafruit_NeoPixel( NUMPIXELS, PIN, NEO_TYPE + NEO_KHZ800 );


// the setup function runs once when you press reset or power the board
void setup()
{
    // Initialize the board (for use with CPL)
    Bsp_Api_initialize();
    Bsp_beginArduinoSerialObject( 115200, SERIAL_8N1 );

    // Initialize CPL
    Cpl::System::Api::initialize();

    // Initialize the NeoPixel shield (and CLEAR all LEDs)
    pixels_.begin();
    for ( int i=0; i < NUMPIXELS; i++ )
    {
        pixels_.setPixelColor( i, pixels_.Color( 0, 0, 0, 0 ) );
    }
    pixels_.show();
    delay( 500 ); // Delay for a period of time (in milliseconds).
}


// the loop function runs over and over again forever
void loop()
{
    // Make the current/main thread a CPL Thread
    Cpl::System::FreeRTOS::Thread::makeNativeMainThreadACplThread();

    // Run the application (Note: This method does not return)
    //runTheApplication( Bsp_Serial(), Bsp_Serial() );
}
