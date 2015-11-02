#ifndef Storm_Type_Pulse_h_
#define Storm_Type_Pulse_h_
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



/// Namespaces
namespace Storm { namespace Type {


/** This class is wrapper to bool data type.  The wrapper's only purpose is
    to reinforce the convention that all Pulse values MUST be reset on start of
    each Processing cycle/interval.
 */
class Pulse
{
protected;
    /// Internal data
    bool m_flag;


public:
    /// Constructor (always to default to NO pulse)
    Pulse(void):m_flag(0){}

public:
    /// Activates the pulse, i.e. sets the flag to true
    inline void pulse(void)         { m_flag = true; };

    /// Deactivates the pulse, i.e. sets the flag to false
    inline void reset(void)         { m_flag = false; };

    /// Returns true if the Pulse has been activated (i.e == true)
    inline bool isPulsed(void)      { return m_flag; }


public:
    /// Assignment
    inline Pulse& operator=( bool newValue )    { m_flag = newValue; return *this; }

    /// Cast operator
    inline operator bool () const               { return m_flag; }



};



};      // end namespace
}; 
#endif  // end header latch
