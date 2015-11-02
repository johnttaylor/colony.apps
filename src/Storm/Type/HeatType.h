#ifndef Storm_Type_HeatType_h_
#define Storm_Type_HeatType_h_
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


/** This class provides the Type defintion for the type/category of a
    Heating source.
 */
class HeatType
{
public:
    /** Heat source types
     */
    enum Enum_T { eNONE       = 0,  //!< No heating source
                  eMECHANICAL = 1,  //!< Compressor heating (aka a Heat pump)
                  eFOSSIL     = 2,  //!< Fossil/Gas furnance
                  eELECTRIC   = 3,  //!< Electric/Strip heat
                  eINVALID    = -1  //!< Invalid selection
                };


    /** Helper method that converts a text string to an enum value.  A pointer
        to the next 'unconverted' character is returned when succesful; else
        zero is returned.  If the text string is invalid, then 'dst' is
        set to eINVALID.  Valid text value are the enumeration symbols, e.g. 
        "eNONE"
     */
    static  const char* toEnum( const char* text, Enum_T& dst ) throw();

    /** Helper method that converts the enum to a text string.  The caller
        must supply the text string memory.
     */
    static const char* toString( Enum_T option, Cpl::Text::String& dst, bool append=false ) throw();
};


};      // end namespace
}; 
#endif  // end header latch
