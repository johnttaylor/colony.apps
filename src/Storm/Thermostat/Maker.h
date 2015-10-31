#ifndef Storm_Thermostat_Maker_h_
#define Storm_Thermostat_Maker_h_
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


#include "Cpl/Itc/MailboxServer.h"
#include "Cpl/Itc/CloseSync.h"

/// Namespaces
namespace Storm { namespace Thermostat {


/** This concrete class is a "Maker" that assembles the objects needed
    for Storm::Thermostat Application's core control algorithms.
 */
class Maker: public Cpl::Itc::MailboxServer,
             public Cpl::Itc::CloseRequest,

{
protected:
    /// Timer that processing cycle for the thermostat algorithm
    Cpl::Timer::Local<Maker)    m_mainLoopTimer;

    /// "main loop" interval time in milliseconds, e.g. 50 = 20Hz
    unsigned long               m_mainLoopResolution;

    /// My global component error state, i.e. if one component fails, all components are consider failed
    bool                        m_enabled;
    
public:
    /// Constructor.
    Maker( unsigned long timingTickInMsec,
           unsigned long mainLoopResolutionInMsec
         )
public:
    /// See Cpl::Itc::OpenRequest
    void request( Cpl::Itc::OpenRequest::OpenMsg& msg );
  
    /// See Cpl::Itc::CloseRequest
    void request( Cpl::Itc::CloseRequest::CloseMsg& msg );


protected:
    /// Helper method

};


};      // end namespace
};      // end namespace
#endif  // end header latch
