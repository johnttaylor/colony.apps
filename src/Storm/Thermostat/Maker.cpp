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
              unsigned long mainLoopResolutionInMsec
            )
:Cpl::Itc::MailboxServer( timingTickInMsec )
,m_mainLoopTimer( *this, *this, &Maker::executeMainLoop )
,m_mainLoopResolution( mainLoopResolutionInMsec )
,m_enabled(false)
    {
    }



///////////////////////////////
void Maker::request( Cpl::Itc::OpenRequest::OpenMsg& msg )
    {
    // Start the main loop timer
    m_mainLoopTimer.start( m_mainLoopResolution );

    // Start my components
    m_enabled = true;
    m_operate.start();
    m_idtPi.start();
    }


void Maker::request( Cpl::Itc::CloseRequest::CloseMsg& msg )
    {
    // Stop the main loop timer
    m_mainLoopTimer.stop();

    // Stop my components
    m_enabled = false;
    m_operate.stop();
    m_idtPi.stop();
    }


///////////////////////////////
void Maker::executeMainLoop
    {
    // Do nothing if I am not enabled, aka do nothing if at least one Component encountered an error
    if ( m_enabled )
        {
        // Restart my main loop interval timer
        m_mainLoopTimer.start( m_mainLoopResolution );

        // Pre Processing
        fetchExternalModelInputs();
        m_dd.beginProcessingCycle();


        // Execute Components (NOTE: ORDER HERE IS IMPORNTANT!)
        Cpl::System::ElaspedTime::Precision_T currentTick = Cpl::System::ElaspedTime::precision();
        m_enabled &= m_operate.do( m_enabled, currentTick );
        m_enabled &= m_idtPi.do( m_enabled, currentTick );


        // Post Processing
        m_dd.endProcessingCycle();
        updateModel();
        }
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