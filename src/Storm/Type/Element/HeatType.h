#ifndef Storm_Type_Element_HeatType_h_
#define Storm_Type_Element_HeatType_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Apps Project.  The Colony.Apps Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.apps/license.txt
*
* Copyright (c) 2015 - 2019 John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

#include "Storm/Type/Element/DataType.h"
#include "Rte/Element/Basic.h"



/// Namespaces
namespace Storm
    {
    namespace Type
        {
        namespace Element
            {
            /** Type definition for the type/category of a Heating source.
                Note: This class is work-around in C++03 for the C++11 'enum class' semantics
            */
            class HeatType_T
                {
                public:
                    /** Type/category of a Heating source.
                    */
                    enum Enum
                        {
                        eNONE       = 0,  //!< No heating source
                        eMECHANICAL = 1,  //!< Compressor heating (aka a Heat pump)
                        eFOSSIL     = 2,  //!< Fossil/Gas furnance
                        eELECTRIC   = 3,  //!< Electric/Strip heat
                        eINVALID    = -1  //!< Invalid selection
                        };
                };

            /** This class define an RTE Element for the Thermostat Options Type
            */
            class HeatType : public Rte::Element::Basic<HeatType_T::Enum, DATATYPE_HEAT>
                {
                public:
                    /// Constructor
                    HeatType( HeatType_T::Enum initialValue = HeatType_T::eNONE,
                              bool             inUse        = false,
                              int8_t           validState   = RTE_ELEMENT_API_STATE_INVALID
                            );

                public:
                    /// See Rte::Element::Api
                    const char* toString( Cpl::Text::String& dstMemory, bool append=false ) const;

                    /// See Rte::Element::Api
                    const char* getTypeAsText( void ) const;


                protected:
                    /// See Rte::Element::Api
                    const char* setFromText( const char* srcText, const char* terminationChars=0 );

                };

            };      // end namespace
        };
    };
#endif  // end header latch
