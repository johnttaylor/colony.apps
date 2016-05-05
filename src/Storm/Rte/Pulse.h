#ifndef Storm_Rte_Pulse_h_
#define Storm_Rte_Pulse_h_
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

#include "Rte/Element/Basic.h"
#include "Storm/Rte/DataType.h"
#include "Storm/Type/Pulse.h"


/// Namespaces
namespace Storm { namespace Rte { 


/** This class define an RTE Element for a 'Pulse', i.e. just a type wrapper
    to Rte::Element::Boolean_T
 */
class Pulse: public  ::Rte::Element::Boolean_T
{
public:
    /// Constructor
    Pulse( bool    initialValue = false,
           bool    inUse        = false,
           int8_t  validState   = RTE_ELEMENT_API_STATE_INVALID
         ): ::Rte::Element::Boolean_T(DATATYPE_PULSE,initialValue,inUse,validState){}

public:
    /// Updates the data content's value
    inline void set( Storm::Type::Pulse newValue )  { m_data = newValue.isPulsed(); }

public:
    /// Activates the pulse, i.e. sets the flag to true
    inline void pulse(void)         { m_data = true; };

    /// Deactivates the pulse, i.e. sets the flag to false
    inline void reset(void)         { m_data = false; };

    /// Returns true if the Pulse has been activated (i.e == true)
    inline bool isPulsed(void)      { return m_data; }


public:
    /// See Rte::Element::Api
    const char* getTypeAsText(void) const           { return "PULSE"; }
};

};      // end namespace
};      
#endif  // end header latch
