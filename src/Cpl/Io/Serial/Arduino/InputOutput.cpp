/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2020  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "InputOutput.h"
#include <stdint.h>

///
using namespace Cpl::Io::Serial::Arduino;



////////////////////////////////////
InputOutput::InputOutput()
    : m_started( false )
{
}


InputOutput::~InputOutput( void )
{
    close();
}


void InputOutput::start( unsigned long baudrate, uint16_t config ) noexcept
{
    if ( !m_started )
    {
        ::Serial.begin( baudrate, config );
        m_started = true;
    }
}

bool InputOutput::isReady( void )
{
    // Fail if I have not been started
    if ( !m_started )
    {
        return false;
    }

    return ::Serial;
}

////////////////////////////////////
bool InputOutput::read( void* buffer, int numBytes, int& bytesRead )
{
    // Fail if I have not been started
    if ( !m_started )
    {
        return false;
    }

    uint8_t* ptr = (uint8_t*) buffer;
    for ( bytesRead = 0; bytesRead < numBytes; bytesRead++ )
    {
        int inbyte = ::Serial.read();
        if ( inbyte < 0 )
        {
            // No more data left in the RX queue -->return
            return true;
        }

        // Store the read-in byte
        *ptr++ = (uint8_t) inbyte;
    }

    // The Serial class NEVER has an 'error' on the read() call
    return true;
}

bool InputOutput::available()
{
    // Fail if I have not been started
    if ( !m_started )
    {
        return false;
    }

    return ::Serial.available();
}


////////////////////////////////////
bool InputOutput::write( const void* buffer, int maxBytes, int& bytesWritten )
{
    // Fail if I have not been started
    if ( !m_started )
    {
        return false;
    }

    const uint8_t* ptr = (const uint8_t*) buffer;
    bytesWritten = maxBytes;

    for ( bytesWritten=0; bytesWritten < maxBytes; bytesWritten++ )
    {
        if ( ::Serial.write( *ptr++ ) != 1 )
        {
            return false;
        }
    }

    return true;
}


void InputOutput::flush()
{
    // Do nothing if I have not been started
    if ( m_started )
    {
        ::Serial.flush();
    }
}

bool InputOutput::isEos()
{
    return !m_started;
}

void InputOutput::close()
{
    // Do nothing if I have not been started
    if ( m_started )
    {
        ::Serial.end();
        m_started = false;
    }
}
