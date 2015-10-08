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
const char* DehumOption::toEnum( const char* text, DehumOption::Enum_T& dst ) throw()
    {
    if ( strcmp( text, "eOFF" ) == 0 )
        {
        dst = eOFF;
        return text + 4;
        }
    else if ( strcmp( text, "eLOW" ) == 0 )
        {
        dst = eLOW;
        return text + 4;
        }
    else if ( strcmp( text, "eMEDIUM" ) == 0 )
        {
        dst = eMEDIUM;
        return text + 7;
        }
    else if ( strcmp( text, "eHIGH" ) == 0 )
        {
        dst = eHIGH;
        return text + 5;
        }

    dst = eINVALID;
    return 0;
    }


const char* DehumOption::toString( DehumOption::Enum_T mode, Cpl::Text::String& dst, bool append ) throw()
    {
    switch( mode )
        {
        case eOFF:    dst.formatOpt( append, "eOFF" );      break;
        case eLOW:    dst.formatOpt( append, "eLOW" );      break;
        case eMEDIUM: dst.formatOpt( append, "eMEDIUM" );   break;
        case eHIGH:   dst.formatOpt( append, "eHIGH" );     break;
        default:      dst.formatOpt( append, "eINVALID" );  break;
        }

    return dst;
    }

