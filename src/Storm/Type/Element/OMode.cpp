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


/// Namespaces
using namespace Storm::Type::Element;


#define NUM_ENUM_ELEMENTS   4
static const char*         enumTexts_[NUM_ENUM_ELEMENTS] = { "eNONE", "eCOOLING", "eHEATING", "eINVALID" };
static const OMode_T::Enum enumVals_[NUM_ENUM_ELEMENTS]  = { OMode_T::eOFF, OMode_T::eCOOLING, OMode_T::eHEATING, OMode_T::eINVALID };

/////////////////////////////
OMode::OMode( OMode_T::Enum initialValue,
              bool          inUse,
              int8_t        validState
              )
    : Rte::Element::Basic<OMode_T::Enum, DATATYPE_OMODE>( initialValue, inUse, validState )
    {
    }

const char* OMode::getTypeAsText( void ) const
    {
    return "OMode";
    }


const char* OMode::toString( Cpl::Text::String& dstMemory, bool append ) const
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


const char* OMode::setFromText( const char* srcText, const char* terminators )
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