#ifndef Storm_Type_h_
#define Storm_Type_h_
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

#include "colony_config.h"
#include "Rte/Element/Basic.h"
#include "Cpl/Text/String.h"


/** The base offset for the RTE DataType magic numbers for all specialized
    datatypes defined in by the STORM namespace (note this is GLOBAL with
    respect to Storm defined RTE DataTypes - NOT just this file). Only 
    override this value if there is collision with other Applicaiton defined 
    RTE DataType magic numbers.
 */
#ifndef OPTION_STORM_TYPE_RTE_DATATYPE_OFFSET
#define OPTION_STORM_TYPE_RTE_DATATYPE_OFFSET  100
#endif


/// Namespaces
namespace Storm {


/** This class provide various Type definitions, enumerations, etc. used
    by the Storm namespace.
 */
class Type
{
public:
    /** Thermostate modes of operation
     */
    enum TMode_T { eOFF     = 0,     //!< No active heating or cooling
                   eCOOLING = 1,     //!< Active cooling
                   eHEATING = 2,     //!< Active heating 
                   eAUTO    = 3,     //!< Active heating and/or cooling (actual mode internally determined)
                   eINVALID = -1,    //!< Invalid selection
                 };


    /** Helper method that converts a text string to an enum fail.  If the text
        string is invalid, then eINVALID is returned.  Valid text value are
        the enumeration symbols, e.g. "eOFF"
     */
    static TMode_T convertTMode_T( const char* text ) throw();

    /** Helpter method that converts the enum to a text string.  The caller
        must supply the text string memory.
     */
    static const char* convertTMode_T( TMode_T mode, Cpl::Text::String& dst ) throw();

     

public: 
    /// RTE DataType magic number for TMode_T
    static const int RTE_DATATYPE_TMODE_T = 0 + Rte::Element::DataType::START_APP_DEFINED + OPTION_STORM_TYPE_RTE_DATATYPE_OFFSET;

    

};


};      // end namespace
#endif  // end header latch
