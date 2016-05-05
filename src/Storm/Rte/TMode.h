#ifndef Storm_Rte_TMode_h_
#define Storm_Rte_TMode_h_
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
/** @file */

#include "Storm/Rte/DataType.h"
#include "Storm/Type/TMode.h"


/// Namespaces
namespace Storm { namespace Rte { 


/** This class define an RTE Element for the Thermostat Options Type
 */
class TMode: public ::Rte::Element::Basic<Storm::Type::TMode::Enum_T, DATATYPE_TMODE>
{
public:
    /// Constructor
    TMode( Storm::Type::TMode::Enum_T initialValue = Storm::Type::TMode::eOFF,
           bool                       inUse        = false,
           int8_t                     validState   = RTE_ELEMENT_API_STATE_INVALID
         );

public:
    /// See Rte::Element::Api
    const char* toString( Cpl::Text::String& dstMemory, bool append=false ) const;

    /// See Rte::Element::Api
    const char* getTypeAsText(void) const;

    
protected:
    /// See Rte::Element::Api
    const char* setFromText( const char* srcText, const char* terminationChars=0 );
    
};

};      // end namespace
};      
#endif  // end header latch
