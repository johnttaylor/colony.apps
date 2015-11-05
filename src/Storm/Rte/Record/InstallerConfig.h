#ifndef Storm_Rte_Record_InstallerConfig_h_
#define Storm_Rte_Record_InstallerConfig_h_
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
#include "Storm/Rte/Point/InstallerConfig.h"
#include "Rte/Db/Record/Basic.h"
#include "Rte/Point/Null.h"
#include "Cpl/Log/Loggers.h"


/// Default Value for PI Cooling Gain#0
#ifndef OPTION_STORM_RTE_RECORD_INSTALLER_COOLING_GAIN_0
#define OPTION_STORM_RTE_RECORD_INSTALLER_COOLING_GAIN_0            15.0f       
#endif

/// Default Value for PI Cooling Reset Time#0 (in milliseconds)
#ifndef OPTION_STORM_RTE_RECORD_INSTALLER_COOLING_RESETTIME_0
#define OPTION_STORM_RTE_RECORD_INSTALLER_COOLING_RESETTIME_0       (5.0f * 60.0f * 1000.0f)
#endif

/// Default Value for PI Heating Gain#0
#ifndef OPTION_STORM_RTE_RECORD_INSTALLER_HEATING_GAIN_0
#define OPTION_STORM_RTE_RECORD_INSTALLER_HEATING_GAIN_0            20.0f
#endif

/// Default Value for PI Heating Reset Time#0 (in milliseconds)
#ifndef OPTION_STORM_RTE_RECORD_INSTALLER_HEATING_RESETTIME_0
#define OPTION_STORM_RTE_RECORD_INSTALLER_HEATING_RESETTIME_0       (5.0f * 60.0f * 1000.0f)
#endif

/// Default Value for PI Cooling Gain#1
#ifndef OPTION_STORM_RTE_RECORD_INSTALLER_COOLING_GAIN_1
#define OPTION_STORM_RTE_RECORD_INSTALLER_COOLING_GAIN_1            20.0f
#endif

/// Default Value for PI Cooling Reset Time#1 (in milliseconds)
#ifndef OPTION_STORM_RTE_RECORD_INSTALLER_COOLING_RESETTIME_1
#define OPTION_STORM_RTE_RECORD_INSTALLER_COOLING_RESETTIME_1       (5.0f * 60.0f * 1000.0f)
#endif

/// Default Value for PI Heating Gain#1
#ifndef OPTION_STORM_RTE_RECORD_INSTALLER_HEATING_GAIN_1
#define OPTION_STORM_RTE_RECORD_INSTALLER_HEATING_GAIN_1            30.0f
#endif

/// Default Value for PI Heating Reset Time#1 (in milliseconds)
#ifndef OPTION_STORM_RTE_RECORD_INSTALLER_HEATING_RESETTIME_1
#define OPTION_STORM_RTE_RECORD_INSTALLER_HEATING_RESETTIME_1       (5.0f * 60.0f * 1000.0f)
#endif




/// Global default delay before updating persistance storage (can also be set/change via the constructor)
#ifndef OPTION_STORM_RTE_RECORD_INSTALLER_UPDATE_DELAY_MSEC
#define OPTION_STORM_RTE_RECORD_INSTALLER_UPDATE_DELAY_MSEC         2000
#endif





/// Namespaces
namespace Storm { namespace Rte { namespace Record {



/** RTE Record for the User operating mode, setpoints, fan mode, etc. a 
    thermostat.
 */
class InstallerConfig: public InstallerConfigModel,
                       public Rte::Db::Record::Base
{
public:
    /// Null Point to be used with the Record's Lite weight Viewer
    Rte::Point::Null<STORM_RTE_POINT_INSTALLERCONFIG_NUM_TUPLES>  m_nullPoint4Viewer;

public:
    /// Constructor
    InstallerConfig( Cpl::Container::Map<ApiLocal>& myRecordList,
                Cpl::Itc::PostApi&             recordLayerMbox, 
                Cpl::Timer::CounterSource&     timingSource,
                unsigned long                  delayWriteTimeInMsec = OPTION_STORM_RTE_RECORD_INSTALLER_UPDATE_DELAY_MSEC,
                Cpl::Log::Api&                 eventLogger          = Cpl::Log::Loggers::application()
              )
    :InstallerConfigModel(recordLayerMbox)
    ,Rte::Db::Record::Base( m_nullPoint4Viewer,
                            *this,
                            myRecordList, 
                            delayWriteTimeInMsec, 
                            "InstallerConfig",
                            recordLayerMbox, 
                            timingSource, 
                            eventLogger 
                          )
        {
        }


public: 
    /// See Rte::Point::Model::Api
    void defaultMe( void ) throw()
        {
        // Default ALL tuples/elements to the VALID state 
        setAllValidState( RTE_ELEMENT_API_STATE_VALID );

        // Default values
        m_piConfig.m_coolingGain0.set( OPTION_STORM_RTE_RECORD_INSTALLER_COOLING_GAIN_0 );
        m_piConfig.m_coolingResetTime0.set( OPTION_STORM_RTE_RECORD_INSTALLER_COOLING_RESETTIME_0 );
        m_piConfig.m_heatingGain0.set( OPTION_STORM_RTE_RECORD_INSTALLER_HEATING_GAIN_0 );
        m_piConfig.m_heatingResetTime0.set( OPTION_STORM_RTE_RECORD_INSTALLER_HEATING_RESETTIME_0 );
        m_piConfig.m_coolingGain1.set( OPTION_STORM_RTE_RECORD_INSTALLER_COOLING_GAIN_1 );
        m_piConfig.m_coolingResetTime1.set( OPTION_STORM_RTE_RECORD_INSTALLER_COOLING_RESETTIME_1 );
        m_piConfig.m_heatingGain1.set( OPTION_STORM_RTE_RECORD_INSTALLER_HEATING_GAIN_1 );
        m_piConfig.m_heatingResetTime1.set( OPTION_STORM_RTE_RECORD_INSTALLER_HEATING_RESETTIME_1 );

        m_equipConfig.m_dehumOption.set( OPTION_STORM_RTE_RECORD_USER_CONFIG_DEHUM_OPTION );

        m_compCoolStage1.m_dehumOption.set( OPTION_STORM_RTE_RECORD_USER_CONFIG_DEHUM_OPTION );
        m_compCoolStage2.m_dehumOption.set( OPTION_STORM_RTE_RECORD_USER_CONFIG_DEHUM_OPTION );
        m_compHeatStage1.m_dehumOption.set( OPTION_STORM_RTE_RECORD_USER_CONFIG_DEHUM_OPTION );
        m_compHeatStage2.m_dehumOption.set( OPTION_STORM_RTE_RECORD_USER_CONFIG_DEHUM_OPTION );
        m_elecHeatStage1.m_dehumOption.set( OPTION_STORM_RTE_RECORD_USER_CONFIG_DEHUM_OPTION );
        m_elecHeatStage2.m_dehumOption.set( OPTION_STORM_RTE_RECORD_USER_CONFIG_DEHUM_OPTION );
        m_elecHeatStage3.m_dehumOption.set( OPTION_STORM_RTE_RECORD_USER_CONFIG_DEHUM_OPTION );
        m_fossilHeatStage1.m_dehumOption.set( OPTION_STORM_RTE_RECORD_USER_CONFIG_DEHUM_OPTION );
        m_fossilHeatStage2.m_dehumOption.set( OPTION_STORM_RTE_RECORD_USER_CONFIG_DEHUM_OPTION );
        m_fossilHeatStage3.m_dehumOption.set( OPTION_STORM_RTE_RECORD_USER_CONFIG_DEHUM_OPTION );
        }
};




    Rte::Element::Float_T       m_coolingGain0
    Rte::Element::Float_T       m_coolingResetTime0
    Rte::Element::Float_T       m_heatingGain0
    Rte::Element::Float_T       m_heatingResetTime0
    Rte::Element::Float_T       m_coolingGain1
    Rte::Element::Float_T       m_coolingResetTime1
    Rte::Element::Float_T       m_heatingGain1
    Rte::Element::Float_T       m_heatingResetTime1


};      // end namespace
};      
};      
#endif  // end header latch
