#ifndef Storm_Type_Record_UserConfig_h_
#define Storm_Type_Record_UserConfig_h_
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
#include "Storm/Type/Point/UserConfig.h"
#include "Rte/Db/Record/Base.h"
#include "Rte/Point/Null.h"
#include "Cpl/Log/Loggers.h"


/// Default Value (can be overridden by the Application)
#ifndef OPTION_STORM_TYPE_RECORD_USER_CONFIG_DEHUM_OPTION
#define OPTION_STORM_TYPE_RECORD_USER_CONFIG_DEHUM_OPTION      DehumOption::eOFF
#endif

/// Default Value (can be overridden by the Application)
#ifndef OPTION_STORM_TYPE_RECORD_USER_CONFIG_DEHUM_SETPOINT
#define OPTION_STORM_TYPE_RECORD_USER_CONFIG_DEHUM_SETPOINT     50.0
#endif

/// Default Value (can be overridden by the Application)
#ifndef OPTION_STORM_TYPE_RECORD_USER_CONFIG_AIR_FILTER_TIME
#define OPTION_STORM_TYPE_RECORD_USER_CONFIG_AIR_FILTER_TIME    60
#endif

/// Default Value (can be overridden by the Application)
#ifndef OPTION_STORM_TYPE_RECORD_USER_CONFIG_DEADBAND
#define OPTION_STORM_TYPE_RECORD_USER_CONFIG_DEADBAND           3.0
#endif

/// Default Value (can be overridden by the Application)
#ifndef OPTION_STORM_TYPE_RECORD_USER_CONFIG_AUTO_MODE_ENABLED
#define OPTION_STORM_TYPE_RECORD_USER_CONFIG_AUTO_MODE_ENABLED  true
#endif


/// Global default delay before updating persistance storage (can also be set/change via the constructor)
#ifndef OPTION_STORM_TYPE_RECORD_USER_CONFIG_UPDATE_DELAY_MSEC
#define OPTION_STORM_TYPE_RECORD_USER_CONFIG_UPDATE_DELAY_MSEC  2000
#endif


/// Namespaces
namespace Storm { namespace Type { namespace Record {



/** RTE Record for the User operating mode, setpoints, fan mode, etc. a 
    thermostat.
 */
class UserConfig: public Storm::Type::Point::UserConfigModel,
                  public Rte::Db::Record::Base
{
public:
    /// Null Point to be used with the Record's Lite weight Viewer
    Rte::Point::Null<STORM_TYPE_POINT_USER_CONFIG_NUM_TUPLES>  m_nullPoint4Viewer;

public:
    /// Constructor
    UserConfig( Cpl::Container::Map<ApiLocal>& myRecordList,
                Cpl::Itc::PostApi&             recordLayerMbox, 
                Cpl::Timer::CounterSource&     timingSource,
                unsigned long                  delayWriteTimeInMsec = OPTION_STORM_TYPE_RECORD_USER_CONFIG_UPDATE_DELAY_MSEC,
                Cpl::Log::Api&                 eventLogger = Cpl::Log::Loggers::application()
              )
    :UserConfigModel(recordLayerMbox)
    ,Rte::Db::Record::Base( m_nullPoint4Viewer,
                            *this,
                            myRecordList, 
                            delayWriteTimeInMsec, 
                            "UserConfig",
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
        m_operate.m_dehumOption.set( OPTION_STORM_TYPE_RECORD_USER_CONFIG_DEHUM_OPTION );
        m_operate.m_dehumSetpoint.set( OPTION_STORM_TYPE_RECORD_USER_CONFIG_DEHUM_SETPOINT );
        m_operate.m_airFilterTime.set( OPTION_STORM_TYPE_RECORD_USER_CONFIG_AIR_FILTER_TIME );
        m_operate.m_deadband.set( OPTION_STORM_TYPE_RECORD_USER_CONFIG_DEADBAND );
        m_operate.m_autoModeEnabled.set( OPTION_STORM_TYPE_RECORD_USER_CONFIG_AUTO_MODE_ENABLED );
        }

};


};      // end namespace
};      
};      
#endif  // end header latch
