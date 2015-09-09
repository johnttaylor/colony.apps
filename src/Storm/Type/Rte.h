#ifndef Storm_Type_Rte_h_
#define Storm_Type_Rte_h_
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
#include "Rte/Element/DataType.h"
#include "Storm/Type/DehumOption.h"
#include "Storm/Type/TMode.h"


/** The base offset for the RTE DataType magic numbers for all specialized
    datatypes defined in by the STORM:TYPE namespace (note this is GLOBAL with
    respect to Storm defined RTE DataTypes - NOT just this file). Only 
    override this value if there is collision with other Applicaiton defined 
    RTE DataType magic numbers.
 */
#ifndef OPTION_STORM_TYPE_RTE_DATATYPE_OFFSET
#define OPTION_STORM_TYPE_RTE_DATATYPE_OFFSET  100
#endif


/// Namespaces
namespace Storm { namespace Type { 

/** This class provide various RTE Type definitions, enumerations, etc. used
    by the Storm namespace.
 */
class Rte
{
public: 
    /// RTE DataType magic number for TMode_T
    static const int DATATYPE_TMODE_T        = 0 + Rte::Element::DataType::START_APP_DEFINED + OPTION_STORM_TYPE_RTE_DATATYPE_OFFSET;

    /// RTE DataType magic number for DehumOption_T
    static const int DATATYPE_DEHUM_OPTION_T = 1 + Rte::Element::DataType::START_APP_DEFINED + OPTION_STORM_TYPE_RTE_DATATYPE_OFFSET;

    

public:
    /// RTE Element type for Thermostat Options
    class TMode_T: public Rte::Element::Basic<TMode::Enum_T, DATATYPE_TMODE_T>
    {
    public:
        /// Constructor
        TMode_T( TMode::Enum_T initialValue = TMode::eOFF,
                 bool          inUse        = false,
                 int8_t        validState   = RTE_ELEMENT_API_STATE_INVALID
               );

    public:
        /// See Rte::Element::Api
        const char* toString( Cpl::Text::String& dstMemory ) const;
    
        /// See Rte::Element::Api
        bool setFromText( const char* srcText );

        /// See Rte::Element::Api
        const char* getTypeAsText(void) const;

    };


public:
    /// RTE Element type for Dehumidify Options
    class DehumOption_T: public Rte::Element::Basic<DehumOption::Enum_T, DATATYPE_DEHUM_OPTION_T>
    {
    public:
        /// Constructor
        DehumOption_T( DehumOption::Enum_T initialValue = DehumOption::eOFF,
                       bool                inUse        = false,
                       int8_t              validState   = RTE_ELEMENT_API_STATE_INVALID
                     );

    public:
        /// See Rte::Element::Api
        const char* toString( Cpl::Text::String& dstMemory ) const;
    
        /// See Rte::Element::Api
        bool setFromText( const char* srcText );
        
        /// See Rte::Element::Api
        const char* getTypeAsText(void) const;
    };
};


};      // end namespace
};      
#endif  // end header latch
