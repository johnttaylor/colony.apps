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


/// Namespaces
using namespace Storm::Rte;


/////////////////////////////
DehumOption::DehumOption( Storm::Type::DehumOption::Enum_T initialValue,
                          bool                inUse,
                          int8_t              validState
                        )
: ::Rte::Element::Basic<Storm::Type::DehumOption::Enum_T, DATATYPE_DEHUM_OPTION>( initialValue, inUse, validState )
    {
    }

const char* DehumOption::getTypeAsText(void) const
    {
    return "DEHUM_OPT";
    }


const char* DehumOption::toString( Cpl::Text::String& dstMemory, bool append ) const
    {
    if ( convertStateToText( dstMemory, append ) )
        {
        Storm::Type::DehumOption::toString( m_data, dstMemory, append );
        }

    return dstMemory; 
    }


const char* DehumOption::setFromText( const char* srcText, const char* terminationChars )
    {
    Storm::Type::DehumOption::Enum_T temp   = Storm::Type::DehumOption::eINVALID;
    const char*                      endPtr = Storm::Type::DehumOption::toEnum( srcText, temp ); 
    if ( endPtr )
        {
        m_data = temp;
        }

    return endPtr;
    }

