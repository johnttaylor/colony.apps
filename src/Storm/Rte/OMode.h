#ifndef Storm_Rte_OMode_h_
#define Storm_Rte_OMode_h_
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

#include "Strom/Rte/DataType.h"
#include "Storm/Type/OMode.h"


/// Namespaces
namespace Storm { namespace Rte { 


/** This class define an RTE Element for the Operating mode type of the Thermostat
 */
class OMode: public Rte::Element::Basic<Storm::Type::OMode::Enum_T, DATATYPE_OMODE>
{
public:
    /// Constructor
    OMode( OMode::Enum_T initialValue = OMode::eOFF,
           bool          inUse        = false,
           int8_t        validState   = RTE_ELEMENT_API_STATE_INVALID
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
