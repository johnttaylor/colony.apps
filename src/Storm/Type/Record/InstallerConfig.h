#ifndef Storm_Type_Record_InstallerConfig_h_
#define Storm_Type_Record_InstallerConfig_h_
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

#include "colony_config.h"
#include "Storm/Type/Point/InstallerConfig.h"
#include "Rte/Db/Record/Base.h"
#include "Rte/Point/Null.h"
#include "Cpl/Log/Loggers.h"


/// Default Value for PI Cooling Gain#0
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_COOLING_GAIN_0
#define OPTION_STORM_TYPE_RECORD_INSTALLER_COOLING_GAIN_0            15.0f       
#endif

/// Default Value for PI Cooling Reset Time#0 (in milliseconds)
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_COOLING_RESETTIME_0
#define OPTION_STORM_TYPE_RECORD_INSTALLER_COOLING_RESETTIME_0       (5.0f * 60.0f * 1000.0f)
#endif

/// Default Value for PI Heating Gain#0
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_HEATING_GAIN_0
#define OPTION_STORM_TYPE_RECORD_INSTALLER_HEATING_GAIN_0            20.0f
#endif

/// Default Value for PI Heating Reset Time#0 (in milliseconds)
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_HEATING_RESETTIME_0
#define OPTION_STORM_TYPE_RECORD_INSTALLER_HEATING_RESETTIME_0       (5.0f * 60.0f * 1000.0f)
#endif

/// Default Value for PI Cooling Gain#1
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_COOLING_GAIN_1
#define OPTION_STORM_TYPE_RECORD_INSTALLER_COOLING_GAIN_1            20.0f
#endif

/// Default Value for PI Cooling Reset Time#1 (in milliseconds)
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_COOLING_RESETTIME_1
#define OPTION_STORM_TYPE_RECORD_INSTALLER_COOLING_RESETTIME_1       (5.0f * 60.0f * 1000.0f)
#endif

/// Default Value for PI Heating Gain#1
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_HEATING_GAIN_1
#define OPTION_STORM_TYPE_RECORD_INSTALLER_HEATING_GAIN_1            30.0f
#endif

/// Default Value for PI Heating Reset Time#1 (in milliseconds)
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_HEATING_RESETTIME_1
#define OPTION_STORM_TYPE_RECORD_INSTALLER_HEATING_RESETTIME_1       (5.0f * 60.0f * 1000.0f)
#endif


/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_NUM_COMPCOOL_STAGES
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_NUM_COMPCOOL_STAGES    1
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_NUM_COMPHEAT_STAGES
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_NUM_COMPHEAT_STAGES    0
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_NUM_ELECHEAT_STAGES
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_NUM_ELECHEAT_STAGES    0
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_NUM_FOSSHEAT_STAGES
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_NUM_FOSSHEAT_STAGES    1
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_PRIMARY_HEAT_TYPE
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_PRIMARY_HEAT_TYPE      Storm::Type::HeatType::eFOSSIL
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_BACKUP_HEAT_TYPE
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_BACKUP_HEAT_TYPE       Storm::Type::HeatType::eNONE
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_VSPD_BLOWER
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_VSPD_BLOWER       false
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_REMOTE_IDT
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_REMOTE_IDT        false
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_IDRH_SENSOR
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_IDRH_SENSOR       false
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_ODT_SENSOR
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_ODT_SENSOR        false
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_ODRH_SENSOR
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_ODRH_SENSOR       false
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_HUMIDSTAT
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_HUMIDSTAT         false
#endif


/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CCST1_CPH
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CCST1_CPH             3
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CCST1_MINON_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CCST1_MINON_TIME      5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CCST1_MINOFF_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CCST1_MINOFF_TIME     5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CCST1_CAP_PERCENT
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CCST1_CAP_PERCENT     1.0
#endif


/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CCST2_CPH
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CCST2_CPH             3
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CCST2_MINON_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CCST2_MINON_TIME      5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CCST2_MINOFF_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CCST2_MINOFF_TIME     5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CCST2_CAP_PERCENT
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CCST2_CAP_PERCENT     1.0
#endif


/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CHST1_CPH
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CHST1_CPH             3
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CHST1_MINON_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CHST1_MINON_TIME      5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CHST1_MINOFF_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CHST1_MINOFF_TIME     5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CHST1_CAP_PERCENT
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CHST1_CAP_PERCENT     1.0
#endif
         

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CHST2_CPH
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CHST2_CPH             3
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CHST2_MINON_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CHST2_MINON_TIME      5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CHST2_MINOFF_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CHST2_MINOFF_TIME     5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_CHST2_CAP_PERCENT
#define OPTION_STORM_TYPE_RECORD_INSTALLER_CHST2_CAP_PERCENT     1.0
#endif


/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_FHST1_CPH
#define OPTION_STORM_TYPE_RECORD_INSTALLER_FHST1_CPH             3
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_FHST1_MINON_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_FHST1_MINON_TIME      5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_FHST1_MINOFF_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_FHST1_MINOFF_TIME     5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_FHST1_CAP_PERCENT
#define OPTION_STORM_TYPE_RECORD_INSTALLER_FHST1_CAP_PERCENT     1.0
#endif


/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_FHST2_CPH
#define OPTION_STORM_TYPE_RECORD_INSTALLER_FHST2_CPH             3
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_FHST2_MINON_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_FHST2_MINON_TIME      5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_FHST2_MINOFF_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_FHST2_MINOFF_TIME     5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_FHST2_CAP_PERCENT
#define OPTION_STORM_TYPE_RECORD_INSTALLER_FHST2_CAP_PERCENT     1.0
#endif


/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_FHST3_CPH
#define OPTION_STORM_TYPE_RECORD_INSTALLER_FHST3_CPH             3
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_FHST3_MINON_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_FHST3_MINON_TIME      5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_FHST3_MINOFF_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_FHST3_MINOFF_TIME     5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_FHST3_CAP_PERCENT
#define OPTION_STORM_TYPE_RECORD_INSTALLER_FHST3_CAP_PERCENT     1.0
#endif


/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_EHST1_CPH
#define OPTION_STORM_TYPE_RECORD_INSTALLER_EHST1_CPH             3
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_EHST1_MINON_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_EHST1_MINON_TIME      5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_EHST1_MINOFF_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_EHST1_MINOFF_TIME     5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_EHST1_CAP_PERCENT
#define OPTION_STORM_TYPE_RECORD_INSTALLER_EHST1_CAP_PERCENT     1.0
#endif


/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_EHST2_CPH
#define OPTION_STORM_TYPE_RECORD_INSTALLER_EHST2_CPH             3
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_EHST2_MINON_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_EHST2_MINON_TIME      5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_EHST2_MINOFF_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_EHST2_MINOFF_TIME     5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_EHST2_CAP_PERCENT
#define OPTION_STORM_TYPE_RECORD_INSTALLER_EHST2_CAP_PERCENT     1.0
#endif


/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_EHST3_CPH
#define OPTION_STORM_TYPE_RECORD_INSTALLER_EHST3_CPH             3
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_EHST3_MINON_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_EHST3_MINON_TIME      5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_EHST3_MINOFF_TIME
#define OPTION_STORM_TYPE_RECORD_INSTALLER_EHST3_MINOFF_TIME     5
#endif

/// Default Value
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_EHST3_CAP_PERCENT
#define OPTION_STORM_TYPE_RECORD_INSTALLER_EHST3_CAP_PERCENT     1.0
#endif



/// Global default delay before updating persistance storage (can also be set/change via the constructor)
#ifndef OPTION_STORM_TYPE_RECORD_INSTALLER_UPDATE_DELAY_MSEC
#define OPTION_STORM_TYPE_RECORD_INSTALLER_UPDATE_DELAY_MSEC         2000
#endif





/// Namespaces
namespace Storm { namespace Type { namespace Record {



/** RTE Record for the User operating mode, setpoints, fan mode, etc. a 
    thermostat.
 */
class InstallerConfig: public Storm::Type::Point::InstallerConfigModel,
                       public Rte::Db::Record::Base
{
public:
    /// Null Point to be used with the Record's Lite weight Viewer
    Rte::Point::Null<STORM_TYPE_POINT_INSTALLERCONFIG_NUM_TUPLES>  m_nullPoint4Viewer;

public:
    /// Constructor
    InstallerConfig( Cpl::Container::Map<ApiLocal>& myRecordList,
                     Cpl::Itc::PostApi&             recordLayerMbox, 
                     Cpl::Timer::CounterSource&     timingSource,
                     unsigned long                  delayWriteTimeInMsec = OPTION_STORM_TYPE_RECORD_INSTALLER_UPDATE_DELAY_MSEC,
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
        m_piConfig.m_coolingGain0.set( OPTION_STORM_TYPE_RECORD_INSTALLER_COOLING_GAIN_0 );
        m_piConfig.m_coolingResetTime0.set( OPTION_STORM_TYPE_RECORD_INSTALLER_COOLING_RESETTIME_0 );
        m_piConfig.m_heatingGain0.set( OPTION_STORM_TYPE_RECORD_INSTALLER_HEATING_GAIN_0 );
        m_piConfig.m_heatingResetTime0.set( OPTION_STORM_TYPE_RECORD_INSTALLER_HEATING_RESETTIME_0 );
        m_piConfig.m_coolingGain1.set( OPTION_STORM_TYPE_RECORD_INSTALLER_COOLING_GAIN_1 );
        m_piConfig.m_coolingResetTime1.set( OPTION_STORM_TYPE_RECORD_INSTALLER_COOLING_RESETTIME_1 );
        m_piConfig.m_heatingGain1.set( OPTION_STORM_TYPE_RECORD_INSTALLER_HEATING_GAIN_1 );
        m_piConfig.m_heatingResetTime1.set( OPTION_STORM_TYPE_RECORD_INSTALLER_HEATING_RESETTIME_1 );

        m_equipConfig.m_numCompCoolingStages.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_NUM_COMPCOOL_STAGES );
        m_equipConfig.m_numCompHeatingStages.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_NUM_COMPHEAT_STAGES );
        m_equipConfig.m_numElecHeatingStages.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_NUM_ELECHEAT_STAGES );
        m_equipConfig.m_numFossilHeatingStages.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_NUM_FOSSHEAT_STAGES );
        m_equipConfig.m_primaryHeatType.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_PRIMARY_HEAT_TYPE );
        m_equipConfig.m_backupHeatType.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_BACKUP_HEAT_TYPE );
        m_equipConfig.m_haveVspdBlower.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_VSPD_BLOWER );
        m_equipConfig.m_haveRemoteIdtSensor.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_REMOTE_IDT );
        m_equipConfig.m_haveIdrhSensor.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_IDRH_SENSOR );
        m_equipConfig.m_haveOdtSensor.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_ODT_SENSOR );
        m_equipConfig.m_haveOdrhSensor.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_ODRH_SENSOR );
        m_equipConfig.m_haveHumidstat.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CONFIG_HAVE_HUMIDSTAT );

        m_compCoolStage1.m_cyclesPerHours.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CCST1_CPH );
        m_compCoolStage1.m_minOnTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CCST1_MINON_TIME );
        m_compCoolStage1.m_minOffTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CCST1_MINOFF_TIME );
        m_compCoolStage1.m_capacityPercent.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CCST1_CAP_PERCENT );

        m_compCoolStage2.m_cyclesPerHours.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CCST2_CPH );
        m_compCoolStage2.m_minOnTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CCST2_MINON_TIME );
        m_compCoolStage2.m_minOffTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CCST2_MINOFF_TIME );
        m_compCoolStage2.m_capacityPercent.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CCST2_CAP_PERCENT );

        m_compHeatStage1.m_cyclesPerHours.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CHST1_CPH );
        m_compHeatStage1.m_minOnTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CHST1_MINON_TIME );
        m_compHeatStage1.m_minOffTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CHST1_MINOFF_TIME );
        m_compHeatStage1.m_capacityPercent.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CHST1_CAP_PERCENT );

        m_compHeatStage2.m_cyclesPerHours.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CHST2_CPH );
        m_compHeatStage2.m_minOnTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CHST2_MINON_TIME );
        m_compHeatStage2.m_minOffTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CHST2_MINOFF_TIME );
        m_compHeatStage2.m_capacityPercent.set( OPTION_STORM_TYPE_RECORD_INSTALLER_CHST2_CAP_PERCENT );

        m_elecHeatStage1.m_cyclesPerHours.set( OPTION_STORM_TYPE_RECORD_INSTALLER_EHST1_CPH );
        m_elecHeatStage1.m_minOnTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_EHST1_MINON_TIME );
        m_elecHeatStage1.m_minOffTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_EHST1_MINOFF_TIME );
        m_elecHeatStage1.m_capacityPercent.set( OPTION_STORM_TYPE_RECORD_INSTALLER_EHST1_CAP_PERCENT );

        m_elecHeatStage2.m_cyclesPerHours.set( OPTION_STORM_TYPE_RECORD_INSTALLER_EHST2_CPH );
        m_elecHeatStage2.m_minOnTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_EHST2_MINON_TIME );
        m_elecHeatStage2.m_minOffTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_EHST2_MINOFF_TIME );
        m_elecHeatStage2.m_capacityPercent.set( OPTION_STORM_TYPE_RECORD_INSTALLER_EHST2_CAP_PERCENT );

        m_elecHeatStage3.m_cyclesPerHours.set( OPTION_STORM_TYPE_RECORD_INSTALLER_EHST3_CPH );
        m_elecHeatStage3.m_minOnTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_EHST3_MINON_TIME );
        m_elecHeatStage3.m_minOffTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_EHST3_MINOFF_TIME );
        m_elecHeatStage3.m_capacityPercent.set( OPTION_STORM_TYPE_RECORD_INSTALLER_EHST3_CAP_PERCENT );

        m_fossilHeatStage1.m_cyclesPerHours.set( OPTION_STORM_TYPE_RECORD_INSTALLER_FHST1_CPH );
        m_fossilHeatStage1.m_minOnTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_FHST1_MINON_TIME );
        m_fossilHeatStage1.m_minOffTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_FHST1_MINOFF_TIME );
        m_fossilHeatStage1.m_capacityPercent.set( OPTION_STORM_TYPE_RECORD_INSTALLER_FHST1_CAP_PERCENT );

        m_fossilHeatStage2.m_cyclesPerHours.set( OPTION_STORM_TYPE_RECORD_INSTALLER_FHST2_CPH );
        m_fossilHeatStage2.m_minOnTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_FHST2_MINON_TIME );
        m_fossilHeatStage2.m_minOffTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_FHST2_MINOFF_TIME );
        m_fossilHeatStage2.m_capacityPercent.set( OPTION_STORM_TYPE_RECORD_INSTALLER_FHST2_CAP_PERCENT );

        m_fossilHeatStage3.m_cyclesPerHours.set( OPTION_STORM_TYPE_RECORD_INSTALLER_FHST3_CPH );
        m_fossilHeatStage3.m_minOnTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_FHST3_MINON_TIME );
        m_fossilHeatStage3.m_minOffTime.set( OPTION_STORM_TYPE_RECORD_INSTALLER_FHST3_MINOFF_TIME );
        m_fossilHeatStage3.m_capacityPercent.set( OPTION_STORM_TYPE_RECORD_INSTALLER_FHST3_CAP_PERCENT );
        }
};



};      // end namespace
};      
};      
#endif  // end header latch
