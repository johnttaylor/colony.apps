#ifndef Storm_Record_Operate_h_
#define Storm_Record_Operate_h_
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

#include "Storm/Point/Operate.h"
#include "Rte/Db/Record/Basic.h"
#include "Rte/Point/Null.h"
#include "Cpl/Log/Loggers.h"


/// Namespaces
namespace Storm { namespace Record {


/** RTE Record for the User operating mode, setpoints, fan mode, etc. a 
    thermostat.
 */
class Operate: public OperateModel,
               public Rte::Db::Record::Base
{
public:
    /// Null Point to be used with the Record's Lite weight Viewer
    Rte::Point::Null<STORM_POINT_OPERATE_NUM_TUPLES>  m_nullPoint4Viewer;

public:
    /// Constructor
    Operate( Cpl::Container::Map<ApiLocal>& myRecordList,
             Cpl::Itc::PostApi&             recordLayerMbox, 
             Cpl::Timer::CounterSource&     timingSource,
             unsigned long                  delayWriteTimeInMsec = 2000,    // Default is: 2 second delay
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
        m_operate.m_coolSetpoint.set( STORM_RECORD_OPERATE_COOL_SETPOINT );
        m_operate.m_heatSetpoint.set( STORM_RECORD_OPERATE_HEAT_SETPOINT );
        m_operate.m_mode.set( STORM_RECORD_OPERATE_MODE );
        m_operate.m_fanCont.set( STORM_RECORD_OPERATE_FAN_CONT );
        m_operate.m_noPrimaryHeat.set( STORM_RECORD_OPERATE_NO_PRIMARY_HEAT );
        }


/// Default Value (can be overridden by the Application)
#ifndef STORM_RECORD_OPERATE_COOL_SETPOINT
#define STORM_RECORD_OPERATE_COOL_SETPOINT      78.0
#endif
/// Default Value (can be overridden by the Application)
#ifndef STORM_RECORD_OPERATE_HEAT_SETPOINT
#define STORM_RECORD_OPERATE_HEAT_SETPOINT      68.0
#endif
/// Default Value (can be overridden by the Application)
#ifndef STORM_RECORD_OPERATE_MODE
#define STORM_RECORD_OPERATE_MODE               Storm::Type::75.0
#endif
/// Default Value (can be overridden by the Application)
#ifndef STORM_RECORD_OPERATE_FAN_CONT
#define STORM_RECORD_OPERATE_FAN_CONT      75.0
#endif
/// Default Value (can be overridden by the Application)
#ifndef STORM_RECORD_OPERATE_NO_PRIMARY_HEAT
#define STORM_RECORD_OPERATE_NO_PRIMARY_HEAT      75.0
#endif

};

};      // end namespace
};      // end namespace
#endif  // end header latch
