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

#define NUM_ENUM_ELEMENTS   4
static const char* enumTexts_[NUM_ENUM_ELEMENTS] ={ "eOFF", "eLOW", "eMEDIUM", "eHIGH" };
static const DehumOption::Enum_T enumVals_[NUM_ENUM_ELEMENTS] = { DehumOption::eOFF, DehumOption::eLOW, DehumOption::eMEDIUM, DehumOption::eHIGH };

/////////////////////////
const char* DehumOption::toEnum( const char* text, DehumOption::Enum_T& dst ) throw()
    {
    for (int i=0; i < NUM_ENUM_ELEMENTS; i++)
        {
        int len = strlen( enumTexts_[i] );
        if (strncmp( text, enumTexts_[i],  len) == 0)
            {
            dst = enumVals_[i];
            return  text+len;
            }
        }

    dst = eINVALID;
    return 0;
    }

const char* DehumOption::toString( DehumOption::Enum_T mode, Cpl::Text::String& dst, bool append ) throw()
    {
    for (int i=0; i < NUM_ENUM_ELEMENTS; i++)
        {
        if (mode == enumVals_[i])
            {
            return enumTexts_[i];
            }
        }

    return "eINVALID";
    }
    

