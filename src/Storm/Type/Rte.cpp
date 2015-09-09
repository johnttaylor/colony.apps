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

#include "Rte.h"


/// Namespaces
using namespace Storm::Type;


/////////////////////////////
Rte::TMode_T::TMode_T( TMode::Enum_T initialValue,
                       bool          inUse,
                       int8_t        validState
                     )
:Rte::Element::Basic<TMode::Enum_T, DATATYPE_TMODE_T>( initialValue, inUse, validState )
    {
    }

const char* Rte::TMode_T::getTypeAsText(void) const
    {
    return "TMODE";
    }


const char* Rte::TMode_T::toString( Cpl::Text::String& dstMemory ) const
    {
    return TMode::convert( m_data, dstMemory );
    }
    

bool Rte::TMode_T::setFromText( const char* srcText )
    {
    TMode::Enum_T temp = Tmode::convert( srcText );
    if ( temp != TMode::eINVALID )
        {
        m_data = temp;
        return true;
        }

    return false;
    }


/////////////////////////////
Rte::DehumOption_T::DehumOption_T( DehumOption::Enum_T initialValue,
                                   bool                inUse,
                                   int8_t              validState
                                 )
:Rte::Element::Basic<DehumOption::Enum_T, DATATYPE_DEHUM_OPTION_T>( initialValue, inUse, validState )
    {
    }


const char* Rte::DehumOption_T::getTypeAsText(void) const
    {
    return "DEHUM_OPT";
    }


const char* Rte::DehumOption_T::toString( Cpl::Text::String& dstMemory ) const
    {
    return DehumOption::convert( m_data, dstMemory );
    }
    

bool Rte::DehumOption_T::setFromText( const char* srcText )
    {
    DehumOption::Enum_T temp = Tmode::convert( srcText );
    if ( temp != DehumOption::eINVALID )
        {
        m_data = temp;
        return true;
        }

    return false;
    }
