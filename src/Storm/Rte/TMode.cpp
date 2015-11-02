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

#include "TMode.h"


/// Namespaces
using namespace Storm::Rte;


/////////////////////////////
Rte::TMode::TMode( TMode::Enum_T initialValue,
                   bool          inUse,
                   int8_t        validState
                 )
:Rte::Element::Basic<Storm::Type::TMode::Enum_T, DATATYPE_TMODE>( initialValue, inUse, validState )
    {
    }

const char* Rte::TMode::getTypeAsText(void) const
    {
    return "TMODE";
    }


const char* Rte::TMode::toString( Cpl::Text::String& dstMemory, bool append ) const
    {
    if ( convertStateToText( dstMemory, append ) )
        {
        Storm::Type::TMode::toString( m_data, dst, append )
        }

    return dstMemory; 
    }


bool Rte::TMode::setFromText( const char* srcText )
    {
    Storm::Type::TMode::Enum_T temp   = Storm::Type::TMode::eINVALID;
    const char*                endPtr = Storm::Type::TMode::toEnum( srcText, temp ); 
    if ( endPtr )
        {
        m_data = temp;
        }

    return endPtr;
    }

