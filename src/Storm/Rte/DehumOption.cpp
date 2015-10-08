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
Rte::DehumOption::DehumOption( DehumOption::Enum_T initialValue,
                               bool                inUse,
                               int8_t              validState
                             )
:Rte::Element::Basic<Storm::Type::DehumOption::Enum_T, DATATYPE_DEHUM_OPTION_T>( initialValue, inUse, validState )
    {
    }

const char* Rte::DehumOption::getTypeAsText(void) const
    {
    return "DEHUM_OPT";
    }


const char* Rte::DehumOption::toString( Cpl::Text::String& dstMemory, bool append ) const
    {
    if ( convertStateToText( dstMemory, append ) )
        {
        Storm::Type::DehumOption::toString( m_data, dst, append )
        }

    return dstMemory; 
    }


bool Rte::DehumOption::setFromText( const char* srcText )
    {
    Storm::Type::DehumOption::Enum_T temp   = Storm::Type::DehumOption::eINVALID;
    const char*                      endPtr = Storm::Type::DehumOption::toEnum( srcText, temp ); 
    if ( endPtr )
        {
        m_data = temp;
        }

    return endPtr;
    }

