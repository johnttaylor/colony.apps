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

#include "DehumOption.h"
#include <string.h>


/// Namespaces
using namespace Storm::Type;


/////////////////////////
Enum_T DehumOption::convert( const char* text ) throw()
    {
    if ( strcmp( text, "eOFF" ) == 0 )
        {
        return eOFF;
        }
    else if ( strcmp( text, "eLOW" ) == 0 )
        {
        return eLOW;
        }
    else if ( strcmp( text, "eMEDIUM" ) == 0 )
        {
        return eMEDIUM;
        }
    else if ( strcmp( text, "eHIGH" ) == 0 )
        {
        return eHIGH;
        }

    return eINVALID;
    }


const char* DehumOption::convert( Enum_T mode, Cpl::Text::String& dst ) throw()
    {
    switch( mode )
        {
        case eOFF:    dst = "eOFF";      break;
        case eLOW:    dst = "eLOW";      break;
        case eMEDIUM: dst = "eMEDIUM";   break;
        case eHIGH:   dst = "eHIGH";     break;
        default:      dst = "eINVALID";  break;
        }

    return dst;
    }

