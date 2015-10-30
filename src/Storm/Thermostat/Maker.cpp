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


#include "Maker.h"


/// Namespaces
using namespace Storm::Thermostat;



///////////////////////////////
Maker::Maker( unsigned long timingTickInMsec,
              unsigned long mainLoopIntervalInMsec
            }
:Cpl::Itc::MailboxServer( timingTickInMsec )
,m_mainLoopTimer( *this, *this, &Maker::executeMainLoop )
,m_mainLoopInterval( mainLoopIntervalInMsec )
    {
    }



///////////////////////////////
void Maker::request( Cpl::Itc::OpenRequest::OpenMsg& msg )
    {
    // Start the main loop timer
    m_mainLoopTimer.start( m_mainLoopInterval );

    // Start my components
    m_componentXXX.start();
    }

void Maker::request( Cpl::Itc::CloseRequest::CloseMsg& msg )
    {
    // Stop the main loop timer
    m_mainLoopTimer.stop();

    // Stop my components
    m_componentXXX.stop();
    }


///////////////////////////////
void Maker::executeMainLoop
    {
    // Restart my main loop interval timer
    m_mainLoopTimer.start( m_mainLoopInterval );

    // Get Data that is NOT owned by the Thermstat Application, i.e. config & runtime inputs
    fetchExternalModelInputs();

    // Execute Components
    m_componentXXX.do();

    // Update the Model with my outputs
    updateModel();
    }


Storm::Rte::Point::OperateQuery      m_operateConfig;
Storm::Rte::Point::UserConfigQuery   m_userConfig;
Storm::Rte::Point::SensorsQuery      m_sensorInputs;

void Maker::fetchExternalModelInputs( void )
    {
    m_operatingConfig.query();
    m_userConfig.query();
    m_sensorInputs.query();
    }