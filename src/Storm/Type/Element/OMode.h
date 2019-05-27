#ifndef Storm_Type_Element_OMode_h_
#define Storm_Type_Element_OMode_h_
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
            /** Type defintion for the operating mode of the thermostat
                 Note: This class is work-around in C++03 for the C++11 'enum class' semantics
            */
            class OMode_T
                {
                public:
                    /** Actual/operating mode of the thermostat.
                     */
                    enum Enum
                        {
                        eOFF     = 0,     //!< No active heating or cooling
                        eCOOLING = 1,     //!< Active cooling
                        eHEATING = 2,     //!< Active heating 
                        eINVALID = -1     //!< Invalid selection
                        };
                };


            /** This class define an RTE Element for the Thermostat Options Type
            */
            class OMode : public Rte::Element::Basic<OMode_T::Enum, DATATYPE_OMODE>
                {
                public:
                    /// Constructor
                    OMode( OMode_T::Enum initialValue = OMode_T::eOFF,
                           bool          inUse        = false,
                           int8_t        validState   = RTE_ELEMENT_API_STATE_INVALID
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
