/*-----------------------------------------------------------------------------
* This file is part of the Colony.Apps Project.  The Colony.Apps Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.apps/license.txt
*
* Copyright (c) 2015 - 2019 John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "DehumOption.h"
#include <string.h>


/// Namespaces
using namespace Storm::Type::Element;

#define NUM_ENUM_ELEMENTS   5
static const char*               enumTexts_[NUM_ENUM_ELEMENTS] = { "eOFF", "eLOW", "eMEDIUM", "eHIGH", "eINVALID" };
static const DehumOption_T::Enum enumVals_[NUM_ENUM_ELEMENTS]  = { DehumOption_T::eOFF, DehumOption_T::eLOW, DehumOption_T::eMEDIUM, DehumOption_T::eHIGH, DehumOption_T::eINVALID };


/////////////////////////////
DehumOption::DehumOption( DehumOption_T::Enum initialValue,
                          bool                inUse,
                          int8_t              validState
                        )
    : Rte::Element::Basic<DehumOption_T::Enum, DATATYPE_DEHUM_OPTION>( initialValue, inUse, validState )
    {
    }

const char* DehumOption::getTypeAsText( void ) const
    {
    return "DEHUM_OPT";
    }


const char* DehumOption::toString( Cpl::Text::String& dstMemory, bool append ) const
    {
    if (convertStateToText( dstMemory, append ))
        {
        const char* textVal = "eINVALID";

        for (int i=0; i < NUM_ENUM_ELEMENTS; i++)
            {
            if (m_data == enumVals_[i])
                {
                textVal = enumTexts_[i];
                }
            }

        dstMemory.formatOpt( append, "%s", textVal );
        }

    return dstMemory;
    }


const char* DehumOption::setFromText( const char* srcText, const char* terminationChars )
    {
    for (int i=0; i < NUM_ENUM_ELEMENTS; i++)
        {
        int len = strlen( enumTexts_[i] );
        if (strncmp( srcText, enumTexts_[i], len ) == 0)
            {
            m_data = enumVals_[i];
            return  srcText + len;
            }
        }

    return 0;
    }


