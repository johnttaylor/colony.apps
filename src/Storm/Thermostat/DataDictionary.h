#ifndef Storm_Thermostat_DataDictionary_h_
#define Storm_Thermostat_DataDictionary_h_
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

#include "Storm/Thermostat/tuples_.h"
#include "Rte/Point/Basic.h"
#include "Rte/Point/Model/Base.h"
#include "Rte/Point/Query/Base.h"
#include "Rte/Point/Query/Tuple.h"
#include "Rte/Point/Viewer/Composer.h"


/// Namespaces
namespace Storm { namespace Thermostat {


/*------------------------- POINT --------------------------------------------*/ 

#define STORM_THERMOSTAT_POINT_DD_LOADVALUE     0       //!< Tuple index
#define STORM_THERMOSTAT_POINT_DD_OPERATE       1       //!< Tuple index
#define STORM_THERMOSTAT_POINT_DD_SYSSTATE      2       //!< Tuple index
#define STORM_THERMOSTAT_POINT_DD_CONFIG        3       //!< Tuple index
#define STORM_THERMOSTAT_POINT_DD_SENSORS       5       //!< Tuple index


/// Number of Tuples in the Point
#define STORM_THERMOSTAT_POINT_DD_NUM_TUPLES    (STORM_THERMOSTAT_POINT_DD_SENSORS+1)


/** RTE Point for the model data that is OWNED by the Thermostat Application.
 */
class DataDictionary: public ::Rte::Point::Basic<STORM_THERMOSTAT_POINT_DD_NUM_TUPLES>
{
public: 
    TupleLoadValue  m_lv;           //!< Tuple
    TupleOperate    m_operate;      //!< Tuple
    TupleSysState   m_sysState;     //!< Tuple
    TupleConfig     m_config;       //!< Tuple 
    TupleSensors    m_sensors;      //!< Tuple 


public:
    /// Constructor
    DataDictionary( void )
        {
        registerTuple( STORM_THERMOSTAT_POINT_DD_LOADVALUE, m_lv );
        registerTuple( STORM_THERMOSTAT_POINT_DD_OPERATE,   m_operate );
        registerTuple( STORM_THERMOSTAT_POINT_DD_SYSSTATE,  m_sysState );
        registerTuple( STORM_THERMOSTAT_POINT_DD_CONFIG,    m_config );
        registerTuple( STORM_THERMOSTAT_POINT_DD_SENSORS,   m_sensors );
        }

public:
    /** This method initializes the DD data at the start of each procesing 
        cycle. Typically this called after the RTE Model is queried, but 
        BEFORE any Component's do() method is called.

        NOTE: This method is 'Private' to the Storm::Thermostat namespace
              and should NOT be called from any other namespaces!
     */
    void beginProcessingCycle_( void )
        {
        m_lv.resetPulses();
        m_operate.resetPulses();
        m_sysState.resetPulses();
        }


};


/*------------------------- MODEL POINTS ------------------------------------*/
/** Model Point for: DataDictionary
 */
class DataDictionaryModel: public DataDictionary,
                           public ::Rte::Point::Model::Base
{
public:
    /// Constructor
    DataDictionaryModel( Cpl::Itc::PostApi& myMbox )
        : ::Rte::Point::Model::Base(*this, myMbox)
            {
            }
};


/*------------------------- QUERY POINTS ------------------------------------*/
/** Query Point: DataDictionary
 */
class DataDictionaryQuery: public DataDictionary,
                           public ::Rte::Point::Query::Base
{
public:
    /// Constructor
    DataDictionaryQuery( DataDictionaryModel& modelPoint, bool initialAllInUseState=true, ::Rte::Point::Model::QueryRequest::Option_T copyOption = ::Rte::Point::Model::QueryRequest::eCOPY )
        : ::Rte::Point::Query::Base(*this, modelPoint, copyOption)
            {
            // Default to querying EVERYTHING
            setAllInUseState(initialAllInUseState);
            }

};



/*------------------------- VIEWER POINTS -----------------------------------*/
/** Viewer Point: DataDictionary
 */
template <class CONTEXT>
class DataDictionaryViewer: public DataDictionary,
                            public ::Rte::Point::Viewer::Composer<CONTEXT>
{
public:
    /// Constructor
    DataDictionaryViewer( CONTEXT&                                                                    context,
                          typename ::Rte::Point::Viewer::Composer<CONTEXT>::ChangeNotificationFunc_T  contextChangedCb,
                          typename ::Rte::Point::Viewer::Composer<CONTEXT>::StoppedNotificationFunc_T contextStoppedCb,
                          DataDictionaryModel&                                                        modelPoint,
                          Cpl::Itc::PostApi&                                                          viewerMbox 
                        )
    : ::Rte::Point::Viewer::Composer<CONTEXT>::Composer(*this, context, contextChangedCb, contextStoppedCb, modelPoint, viewerMbox)
        {}
};


/** LIGHT WEIGHT Viewer Point: DataDictionary
 */
//template <class CONTEXT>
//class DataDictionaryLViewer: public ::Rte::Point::Null<STORM_TYPE_POINT_OPERATE_NUM_TUPLES>,
//                             public ::Rte::Point::Viewer::Composer<CONTEXT>
//{
//public:
//    /// Constructor
//    DataDictionaryLViewer( CONTEXT&                                                                    context,
//                           typename ::Rte::Point::Viewer::Composer<CONTEXT>::ChangeNotificationFunc_T  contextChangedCb,
//                           typename ::Rte::Point::Viewer::Composer<CONTEXT>::StoppedNotificationFunc_T contextStoppedCb,
//                           DataDictionaryModel&                                                        modelPoint,
//                           Cpl::Itc::PostApi&                                                          viewerMbox 
//                         )
//    : ::Rte::Point::Viewer::Composer<CONTEXT>::Composer(*this, context, contextChangedCb, contextStoppedCb, modelPoint, viewerMbox)
//        {}
//};



};      // end namespace
};      
#endif  // end header latch
