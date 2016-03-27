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
Maker::Maker( unsigned long                             timingTickInMsec,
              unsigned long                             mainLoopResolutionInMsec,
              Cpl::Itc::PostApi&                        mboxForDataDictionaryModel,
              Storm::Rte::Point::OperateModel&          operateModel,
              Storm::Rte::Point::UserConfigModel&       userConfigModel,
              Storm::Rte::Point::InstallerConfigModel&  installerConfigModel,
              Storm::Rte::Point::SensorsModel&          sensorsModel
            )
:Cpl::Itc::MailboxServer( timingTickInMsec )
,m_mainLoopTimer( *this, *this, &Maker::executeMainLoop )
,m_mainLoopResolution( mainLoopResolutionInMsec )
,m_enabled( false )
,m_dd( m_ddModel )
,m_ddModel( mboxForDataDictionaryModel )
// 
,m_preProcessConfig( m_dd, m_qry_operateConfig )
,m_preProcessSensors( m_dd, m_qry_operateConfig, m_qry_sensorInputs )
,m_operatingMode( m_dd, m_qry_operateConfig, m_qry_sensorInputs )
,m_piContextIdt( m_dd, m_qry_operateConfig, m_qry_userConfig, m_qry_sensorInputs )
,m_pi( m_dd ),
//
,m_qry_operateConfig( operateModel )
,m_qry_userConfig( userConfigModel )
,m_qry_installerConfig( installerConfigModel )
,m_qry_sensorInputs( sensorsModel )
    {
    // Config my DD Controller to update ALL Tuples/Elements!
    m_dd.setAllInUse( true );
    }


DictionaryModel& Maker::getDDModel( void )
    {
    return m_ddModel;
    }


///////////////////////////////
void Maker::request( Cpl::Itc::OpenRequest::OpenMsg& msg )
    {
    // Start the main loop timer
    m_mainLoopTimer.start( m_mainLoopResolution );

    // Start my components
    m_enabled = true;
    m_operatingMode.start();
    m_piContextIdt.start();
    m_piIdt.start();
    }


void Maker::request( Cpl::Itc::CloseRequest::CloseMsg& msg )
    {
    // Stop the main loop timer
    m_mainLoopTimer.stop();

    // Invalidate all of my OWNED model data
    m_dd.setAllInvalidState( RTE_ELEMENT_API_STATE_INVALID );
    m_dd.updateModel();

    // Stop my components
    m_enabled = false;
    m_operatingMode.stop();
    m_piContextIdt.stop();
    m_piIdt.stop();
    }


///////////////////////////////
void Maker::executeMainLoop
    {
    // Restart my main loop interval timer
    m_mainLoopTimer.start( m_mainLoopResolution );

    // Pre-Processing
    m_dd.beginProcessingCycle_();
    m_qry_operatingConfig.query();
    m_qry_userConfig.query();
    m_qry_installerConfig.query();
    m_qry_sensorInputs.query();


    // Execute Components (NOTE: ORDER HERE IS IMPORNTANT!)
    Cpl::System::ElaspedTime::Precision_T currentTick = Cpl::System::ElaspedTime::precision();
    m_enabled &= m_preProcessConfig.do( m_enabled, currentTick );
    m_enabled &= m_preProcessSensors.do( m_enabled, currentTick );
    m_enabled &= m_operatingMode.do( m_enabled, currentTick );
    m_enabled &= m_piContextIdt.do( m_enabled, currentTick );
    m_enabled &= m_piIdt.do( m_enabled, currentTick );


    // Post Processing (only update the model if NO ERRORS!)
    if ( m_enabled )
        {
        m_dd.updateModel();
        }
    }

