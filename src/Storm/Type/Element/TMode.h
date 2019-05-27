#ifndef Storm_Type_Element_TMode_h_
#define Storm_Type_Element_TMode_h_
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
            /** Defintion for the thermostat mode as presented to the  end user.
                Note: This class is work-around in C++03 for the C++11 'enum class' semantics
             */
            class TMode_T
                {
                public:
                    /** Thermostat mode as presented to the end user.
                      */
                    enum Enum
                        {
                        eOFF     = 0,     //!< No active heating or cooling
                        eCOOLING = 1,     //!< Active cooling
                        eHEATING = 2,     //!< Active heating 
                        eAUTO    = 3,     //!< Active heating and/or cooling (actual mode internally determined)
                        eINVALID = -1     //!< Invalid selection
                        };
                };

            /** This class define an RTE Element for the Thermostat Options Type
             */
            class TMode : public Rte::Element::Basic<TMode_T::Enum, DATATYPE_TMODE>
                {
                public:
                    /// Constructor
                    TMode( TMode_T::Enum initialValue = TMode_T::eOFF,
                           bool                        inUse        = false,
                           int8_t                      validState   = RTE_ELEMENT_API_STATE_INVALID
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
