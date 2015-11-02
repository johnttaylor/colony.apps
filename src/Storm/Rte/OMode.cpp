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
using namespace Storm::Rte;


/////////////////////////////
Rte::OMode::OMode( OMode::Enum_T initialValue,
                   bool          inUse,
                   int8_t        validState
                 )
:Rte::Element::Basic<Storm::Type::OMode::Enum_T, DATATYPE_OMODE>( initialValue, inUse, validState )
    {
    }

const char* Rte::OMode::getTypeAsText(void) const
    {
    return "OMODE";
    }


const char* Rte::OMode::toString( Cpl::Text::String& dstMemory, bool append ) const
    {
    if ( convertStateToText( dstMemory, append ) )
        {
        Storm::Type::OMode::toString( m_data, dst, append )
        }

    return dstMemory; 
    }


bool Rte::OMode::setFromText( const char* srcText )
    {
    Storm::Type::OMode::Enum_T temp   = Storm::Type::OMode::eINVALID;
    const char*                endPtr = Storm::Type::OMode::toEnum( srcText, temp ); 
    if ( endPtr )
        {
        m_data = temp;
        }

    return endPtr;
    }

