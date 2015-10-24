#ifndef Storm_Type_OMode_h_
#define Storm_Type_OMode_h_
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


/** This class provides the Type defintion for the actual/operating mode of 
    the thermostat.
 */
class OMode
{
public:
    /** Thermostate modes of operation
     */
    enum Enum_T { eOFF     = 0,     //!< No active heating or cooling
                  eCOOLING = 1,     //!< Active cooling
                  eHEATING = 2,     //!< Active heating 
                  eINVALID = -1     //!< Invalid selection
                };


    /** Helper method that converts a text string to an enum value.  A pointer
        to the next 'unconverted' character is returned when succesful; else
        zero is returned.  If the text string is invalid, then 'dst' is
        set to eINVALID.  Valid text value are the enumeration symbols, e.g. 
        "eOFF"
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
