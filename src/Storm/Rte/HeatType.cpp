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


/// Namespaces
using namespace Storm::Rte;


/////////////////////////////
Rte::HeatType::HeatType( HeatType::Enum_T initialValue,
                         bool             inUse,
                         int8_t           validState
                       )
:Rte::Element::Basic<Storm::Type::HeatType::Enum_T, DATATYPE_HEAT_TYPE>( initialValue, inUse, validState )
    {
    }

const char* Rte::HeatType::getTypeAsText(void) const
    {
    return "HEAT_TYPE";
    }


const char* Rte::HeatType::toString( Cpl::Text::String& dstMemory, bool append ) const
    {
    if ( convertStateToText( dstMemory, append ) )
        {
        Storm::Type::HeatType::toString( m_data, dst, append )
        }

    return dstMemory; 
    }


bool Rte::HeatType::setFromText( const char* srcText )
    {
    Storm::Type::HeatType::Enum_T temp   = Storm::Type::HeatType::eINVALID;
    const char*                   endPtr = Storm::Type::HeatType::toEnum( srcText, temp ); 
    if ( endPtr )
        {
        m_data = temp;
        }

    return endPtr;
    }

