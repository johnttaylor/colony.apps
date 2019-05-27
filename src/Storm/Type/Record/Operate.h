#ifndef Storm_Type_Record_Operate_h_
#define Storm_Type_Record_Operate_h_
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
#include "Storm/Rte/Point/Operate.h"
#include "Rte/Db/Record/Basic.h"
#include "Rte/Point/Null.h"
#include "Cpl/Log/Loggers.h"



/// Default Value (can be overridden by the Application)
#ifndef OPTION_STORM_TYPE_RECORD_OPERATE_COOL_SETPOINT
#define OPTION_STORM_TYPE_RECORD_OPERATE_COOL_SETPOINT      78.0
#endif

/// Default Value (can be overridden by the Application)
#ifndef OPTION_STORM_TYPE_RECORD_OPERATE_HEAT_SETPOINT
#define OPTION_STORM_TYPE_RECORD_OPERATE_HEAT_SETPOINT      68.0
#endif

/// Default Value (can be overridden by the Application)
#ifndef OPTION_STORM_TYPE_RECORD_OPERATE_MODE
#define OPTION_STORM_TYPE_RECORD_OPERATE_MODE               Storm::Type::TMode::eOFF
#endif

/// Default Value (can be overridden by the Application)
#ifndef OPTION_STORM_TYPE_RECORD_OPERATE_FAN_CONT
#define OPTION_STORM_TYPE_RECORD_OPERATE_FAN_CONT           false
#endif

/// Default Value (can be overridden by the Application)
#ifndef OPTION_STORM_TYPE_RECORD_OPERATE_NO_PRIMARY_HEAT
#define OPTION_STORM_TYPE_RECORD_OPERATE_NO_PRIMARY_HEAT    false
#endif


/// Global default delay before updating persistance storage (can also be set/change via the constructor)
#ifndef OPTION_STORM_TYPE_RECORD_OPERATE_UPDATE_DELAY_MSEC
#define OPTION_STORM_TYPE_RECORD_OPERATE_UPDATE_DELAY_MSEC  2000
#endif


/// Namespaces
namespace Storm { namespace Rte { namespace Record {



/** RTE Record for the User operating mode, setpoints, fan mode, etc. a 
    thermostat.
 */
class Operate: public OperateModel,
               public Rte::Db::Record::Base
{
public:
    /// Null Point to be used with the Record's Lite weight Viewer
    Rte::Point::Null<STORM_TYPE_POINT_OPERATE_NUM_TUPLES>  m_nullPoint4Viewer;

public:
    /// Constructor
    Operate( Cpl::Container::Map<ApiLocal>& myRecordList,
             Cpl::Itc::PostApi&             recordLayerMbox, 
             Cpl::Timer::CounterSource&     timingSource,
             unsigned long                  delayWriteTimeInMsec = OPTION_STORM_TYPE_RECORD_OPERATE_UPDATE_DELAY_MSEC,
             Cpl::Log::Api&                 eventLogger = Cpl::Log::Loggers::application()
           )
    :OperateModel(recordLayerMbox)
    ,Rte::Db::Record::Base( m_nullPoint4Viewer,
                            *this,
                            myRecordList, 
                            delayWriteTimeInMsec, 
                            "Operate",
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
        m_operate.m_coolSetpoint.set( OPTION_STORM_TYPE_RECORD_OPERATE_COOL_SETPOINT );
        m_operate.m_heatSetpoint.set( OPTION_STORM_TYPE_RECORD_OPERATE_HEAT_SETPOINT );
        m_operate.m_mode.set( OPTION_STORM_TYPE_RECORD_OPERATE_MODE );
        m_operate.m_fanCont.set( OPTION_STORM_TYPE_RECORD_OPERATE_FAN_CONT );
        m_operate.m_noPrimaryHeat.set( OPTION_STORM_TYPE_RECORD_OPERATE_NO_PRIMARY_HEAT );
        }



};

};      // end namespace
};      
};      
#endif  // end header latch
