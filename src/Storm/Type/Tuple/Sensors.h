#ifndef Storm_Type_Tuple_Sensors_h_
#define Storm_Type_Tuple_Sensors_h_
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

#include "Rte/Tuple/Basic.h"
#include "Rte/Element/Basic.h"


#define STORM_TYPE_TUPLE_SENSORS_IDRH        0       //!< Element Index
#define STORM_TYPE_TUPLE_SENSORS_ODRH        1       //!< Element Index
#define STORM_TYPE_TUPLE_SENSORS_IDT         2       //!< Element Index
#define STORM_TYPE_TUPLE_SENSORS_RIDT        3       //!< Element Index
#define STORM_TYPE_TUPLE_SENSORS_ODT         4       //!< Element Index
#define STORM_TYPE_TUPLE_SENSORS_NUM_TUPLES  (STORM_TYPE_TUPLE_SENSORS_ODT+1)  //!< Number of Elements



/// Namespaces
namespace Storm
    {
    namespace Type
        {
        namespace Tuple
            {
            /** Tuple for the User configurable parameters for a thermostat
             */
            class Sensors : public Rte::Tuple::Basic<STORM_TYPE_TUPLE_SENSORS_NUM_TUPLES>
                {
                public:
                    /// Current indoor relative humidity (% humidity)
                    Rte::Element::Float           m_idrh;

                    /// Current outdoor relative humidity (% humidity)
                    Rte::Element::Float           m_odrh;

                    /// Current indoor temperature in degrees Fahrenheit 
                    Rte::Element::Float           m_idt;

                    /// Current Remote indoor temperature in degrees Fahrenheit 
                    Rte::Element::Float           m_ridt;

                    /// Current outdoor temperature in degrees Fahrenheit 
                    Rte::Element::Float           m_odt;


                public:
                    /// Constructor
                    Sensors( void )
                        {
                        registerElement( STORM_TYPE_TUPLE_SENSORS_IDRH, m_idrh );
                        registerElement( STORM_TYPE_TUPLE_SENSORS_ODRH, m_odrh );
                        registerElement( STORM_TYPE_TUPLE_SENSORS_IDT, m_idt );
                        registerElement( STORM_TYPE_TUPLE_SENSORS_RIDT, m_ridt );
                        registerElement( STORM_TYPE_TUPLE_SENSORS_ODT, m_odt );
                        }

                };


            };      // end namespace
        };
    };
#endif  // end header latch
