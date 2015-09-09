#ifndef Storm_Type_DehumOption_h_
#define Storm_Type_DehumOption_h_
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

#include "Cpl/Text/String.h"


/// Namespaces
namespace Storm { namespace Type { 

/** This class provide Type definition for Dehumidify options.
 */
class DehumOption
{
public:
    /** Strength (i.e. how aggressive) the dehumify algorithm(s)
     */
    enum Enum_T { eOFF     = 0,    //!< All dehumify algorithm(s) are disabled
                  eLOW     = 1,    //!< Minimum dehumify actions
                  eMEDIUM  = 2,    //!< Nominal/recommended dehumidify actions
                  eHIGH    = 3,    //!< Enable all/most-aggressive dehumidfy actions
                  eINVALID = -1    //!< Invalid selection
                };


    /** Helper method that converts a text string to an enum fail.  If the text
        string is invalid, then eINVALID is returned.  Valid text value are
        the enumeration symbols, e.g. "eOFF"
     */
    static Enum_T convert( const char* text ) throw();

    /** Helper method that converts the enum to a text string.  The caller
        must supply the text string memory.
     */
    static const char* convert( Enum_T option, Cpl::Text::String& dst ) throw();
};


};      // end namespace
}; 
#endif  // end header latch
