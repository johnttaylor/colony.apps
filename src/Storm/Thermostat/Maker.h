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

#include "colony_config.h"
#include "Cpl/Itc/MailboxServer.h"
#include "Cpl/Itc/CloseSync.h"
#include "Storm/Thermostat/Api.h"
#include "Storm/Thermostat/DataDictionary.h"
#include "Storm/Thermostat/components_.h"



/** The default interval time, in seconds, for the cycle proccessing of the
    Application.
 */
#ifndef STORM_THERMOSTAT_MAKER_INTERVAL_TIME_SEC
#define STORM_THERMOSTAT_MAKER_INTERVAL_TIME_SEC      2
#endif

/** The default interval time, in milliseoncds, for the cycle proccessing of the
    Application.
 */
#ifndef STORM_THERMOSTAT_MAKER_INTERVAL_TIME_MSEC
#define STORM_THERMOSTAT_MAKER_INTERVAL_TIME_MSEC     0
#endif



/// Namespaces
namespace Storm { namespace Thermostat {


/** This concrete class is a "Maker" that assembles the objects needed
    for Storm::Thermostat Application's core control algorithms.
 */
class Maker: public Cpl::Itc::MailboxServer,
             public Cpl::Itc::CloseRequest,
             public Api

{
public:
    /// Common Interval processing time for the Thermostat Application
    static const Cpl::System::ElaspedTime::Precision_T m_interval = { STORM_THERMOSTAT_MAKERINTERVAL_TIME, STORM_THERMOSTAT_MAKER_INTERVAL_TIME_MSEC };


protected:
    /// Timer that processing cycle for the thermostat algorithm
    Cpl::Timer::Local<Maker>    m_mainLoopTimer;

    /// "main loop" interval time in milliseconds, e.g. 50 = 20Hz
    unsigned long               m_mainLoopResolution;

    /// My global component error state, i.e. if one component fails, all components are consider failed
    bool                        m_enabled;

    /// Point data that is OWNED by this Application
    DataDictionaryController    m_dd;

    /// Actual Model Point (for my OWNED data)
    DataDiciontaryModel         m_ddModel;


protected: // Components
    OperatingMode               m_operatingMode;    //!< Component
    PiContextIdt                m_piContextIdt;     //!< Component
    Pi                          m_piIdt;            //!< Component


protected: // Model Inputs
    Storm::Rte::Point::OperateQuery      m_qry_operateConfig;   //!< RTE Model Query
    Storm::Rte::Point::UserConfigQuery   m_qry_userConfig;      //!< RTE Model Query
    Storm::Rte::Point::SensorsQuery      m_qry_sensorInputs;    //!< RTE Model Query


public:
    /// Constructor.
    Maker( unsigned long                        timingTickInMsec,
           unsigned long                        mainLoopResolutionInMsec,
           Cpl::Itc::PostApi&                   mboxForDataDictionaryModel,
           Storm::Rte::Point::OperateModel&     operateModel,
           Storm::Rte::Point::UserConfigModel&  userConfigModel,
           Storm::Rte::Point::SensorsModel&     sensorsModel
         )

public:
    /// See Storm::Thermostat::Api
    DictionaryModel& getDDModel( void ); 

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
