/*-----------------------------------------------------------------------------
* This file is part of the Colony.Apps Project.  The Colony.Apps Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.apps/license.txt
*
* Copyright (c) 2015 John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "OMode.h"
#include <string.h>


/// Namespaces
using namespace Storm::Type;


/////////////////////////
Enum_T OMode::convert( const char* text ) throw()
    {
    if ( strcmp( text, "eOFF" ) == 0 )
        {
        return eOFF;
        }
    else if ( strcmp( text, "eCOOLING" ) == 0 )
        {
        return eCOOLING;
        }
    else if ( strcmp( text, "eHEATING" ) == 0 )
        {
        return eHEATING;
        }

    return eINVALID;
    }


const char* OMode::convert( Enum_T mode, Cpl::Text::String& dst ) throw()
    {
    switch( mode )
        {
        case eOFF:      dst = "eOFF";       break;
        case eCOOLING:  dst = "eCOOLING";   break;
        case eHEATING:  dst = "eHEATING";   break;
        default:        dst = "eINVALID";   break;
        }

    return dst;
    }

