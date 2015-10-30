#ifndef Storm_Rte_Point_Sensors_h_
#define Storm_Rte_Point_Sensors_h_
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

#include "Storm/Rte/Tuple/Sensors.h"
#include "Rte/Point/Basic.h"
#include "Rte/Point/Model/Base.h"
#include "Rte/Point/Controller/Base.h"
#include "Rte/Point/Controller/RmwComposer.h"
#include "Rte/Point/Controller/Tuple.h"
#include "Rte/Point/Query/Base.h"
#include "Rte/Point/Query/Tuple.h"
#include "Rte/Point/Viewer/Composer.h"


/// Namespaces
namespace Storm { namespace Rte { namespace Point {


/*------------------------- POINTS ------------------------------------------*/ 

/// Tuple index for: Sensors
#define STORM_RTE_POINT_SENSORS_SENSORS     0

/// Number of Tuples in the Point
#define STORM_RTE_POINT_SENSORS_NUM_TUPLES  (STORM_RTE_POINT_SENSORS_SENSORS+1)



/** RTE Point for the User operating mode, setpoints, fan mode, etc. a 
    thermostat.
 */
class Sensors: public Rte::Point::Basic<STORM_RTE_POINT_SENSORS_NUM_TUPLES>
{
public: 
    /// Sensors Tuple
    Storm::Tuple::Sensors m_sensors;


public:
    /// Constructor
    Sensors( void )
        {
        registerTuple( STORM_RTE_POINT_SENSORS_SENSORS, m_operate );
        }

};


/*------------------------- MODEL POINTS ------------------------------------*/
/** Model Point for: Sensors
 */
class SensorsModel: public Sensors,
                    public Rte::Point::Model::Base
{
public:
    /// Constructor
    SensorsModel( Cpl::Itc::PostApi& myMbox )
        :Rte::Point::Model::Base(*this, myMbox)
            {
            }
};


/*------------------------- CONTROLLER POINTS -------------------------------*/
/** Controller Point: Sensors
 */
class SensorsController: public Sensors,
                         public Rte::Point::Controller::Base
{
public:
    /// Constructor
    SensorsController( SensorsModel& modelPoint )
        :Rte::Point::Controller::Base(*this, modelPoint)
            {
            }

};


/** Tuple Controller Point: Sensors
 */
class SensorsTupleController: public Sensors
                              public Rte::Point::Controller::Tuple
{
public:
    /// Constructor
    SensorsTupleController( SensorsModel& modelPoint, unsigned myTupleItemIdx = 0 )
        :Rte::Point::Controller::Tuple(myTupleItemIdx, *this, modelPoint)
            {
            }

};


/** Read-Modify-Write Controller Point: Sensors
 */
template <class CONTEXT>
class SensorsRmwController: public Point::Sensors,
                            public Rte::Point::Controller::RmwComposer<CONTEXT>
{
protected:
    /// Constructor.
    SensorsRmwController( CONTEXT&                                                            context, 
                          typename Rte::Point::Controller::RmwComposer<CONTEXT>::ModifyFunc_T modifyCallback, 
                          SensorsModel&                                                       modelPoint 
                        )
        :Rte::Point::Controller::RmwComposer<CONTEXT>(*this, context, modifyCallback, modelPoint )
            {}
};


/*------------------------- QUERY POINTS ------------------------------------*/
/** Query Point: Sensors
 */
class SensorsQuery: public Sensors,
                    public Rte::Point::Query::Base
{
public:
    /// Constructor
    SensorsQuery( SensorsModel& modelPoint, bool initialAllInUseState=true, Rte::Point::Model::QueryRequest::Option_T copyOption = Rte::Point::Model::QueryRequest::eCOPY )
        :Rte::Point::Query::Base(*this, modelPoint, copyOption)
            {
            // Default to querying EVERYTHING
            setAllInUseState(initialAllInUseState);
            }

};

/** Tuple Query Point: Sensors (Single Tuple, no traversal)
 */
class SensorsQueryTuple: public Sensors, 
                         public Rte::Point::Query::Tuple
{
public:
    /// Constructor
    SensorsQueryTuple( SensorsModel&                             modelPoint, 
                       unsigned                                  tupleIndex = 0, 
                       Rte::Point::Model::QueryRequest::Option_T copyOption = Rte::Point::Model::QueryRequest::eCOPY 
                     )
        :Rte::Point::Query::Tuple(tupleIndex, *this, modelPoint, copyOption )
            {
            // Default to querying EVERYTHING
            setAllInUseState(true);
            }

};



/*------------------------- VIEWER POINTS -----------------------------------*/
/** Viewer Point: Sensors
 */
template <class CONTEXT>
class SensorsViewer: public Sensors,
                     public Rte::Point::Viewer::Composer<CONTEXT>
{
public:
    /// Constructor
    SensorsViewer( CONTEXT&                                                                    context,
                   typename Rte::Point::Viewer::Composer<CONTEXT>::ChangeNotificationFunc_T    contextChangedCb,
                   typename Rte::Point::Viewer::Composer<CONTEXT>::StoppedNotificationFunc_T   contextStoppedCb,
                   SensorsModel&                                                               modelPoint,
                   Cpl::Itc::PostApi&                                                          viewerMbox 
                 )
    :Rte::Point::Viewer::Composer<CONTEXT>::Composer(*this, context, contextChangedCb, contextStoppedCb, modelPoint, viewerMbox)
        {}
};


/** LIGHT WEIGHT Viewer Point: Sensors
 */
template <class CONTEXT>
class SensorsLViewer: public Rte::Point::Null<STORM_RTE_POINT_SENSORS_NUM_TUPLES>
                      public Rte::Point::Viewer::Composer<CONTEXT>
{
public:
    /// Constructor
    SensorsLViewer( CONTEXT&                                                                    context,
                    typename Rte::Point::Viewer::Composer<CONTEXT>::ChangeNotificationFunc_T    contextChangedCb,
                    typename Rte::Point::Viewer::Composer<CONTEXT>::StoppedNotificationFunc_T   contextStoppedCb,
                    SensorsModel&                                                               modelPoint,
                    Cpl::Itc::PostApi&                                                          viewerMbox 
                  )
    :Rte::Point::Viewer::Composer<CONTEXT>::Composer(*this, context, contextChangedCb, contextStoppedCb, modelPoint, viewerMbox)
        {}
};



};      // end namespace
};      
};      
#endif  // end header latch
