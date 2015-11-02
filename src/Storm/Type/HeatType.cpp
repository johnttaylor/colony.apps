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

#include "HeatType.h"
#include <string.h>


/// Namespaces
using namespace Storm::Type;


/////////////////////////
Enum_T HeatType::convert( const char* text ) throw()
    {
    if ( strcmp( text, "eNONE" ) == 0 )
        {
        return eNONE;
        }
    else if ( strcmp( text, "eMECHANICAL" ) == 0 )
        {
        return eMECHANICAL;
        }
    else if ( strcmp( text, "eFOSSIL" ) == 0 )
        {
        return eFOSSIL;
        }
    else if ( strcmp( text, "eELECTRIC" ) == 0 )
        {
        return eELECTRIC;
        }

    return eINVALID;
    }


const char* HeatType::convert( Enum_T mode, Cpl::Text::String& dst ) throw()
    {
    switch( mode )
        {
        case eNONE:         dst = "eNONE";          break;
        case eMECHANICAL:   dst = "eMECHANICAL";    break;
        case eFOSSIL:       dst = "eFOSSIL";        break;
        case eELECTRIC:     dst = "eELECTRIC";      break;
        default:            dst = "eINVALID";       break;
        }

    return dst;
    }

