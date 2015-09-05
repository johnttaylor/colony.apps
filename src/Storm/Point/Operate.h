#ifndef Storm_Point_Operate_h_
#define Storm_Point_Operate_h_
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

#include "Storm/Tupe/Operate.h"
#include "Rte/Point/Basic.h"
#include "Rte/Point/Model/Base.h"
#include "Rte/Point/Controller/Base.h"
#include "Rte/Point/Controller/RmwComposer.h"
#include "Rte/Point/Query/Base.h"
#include "Rte/Point/Viewer/Composer.h"


/// Namespaces
namespace Storm { namespace Point {


/*------------------------- POINTS ------------------------------------------*/ 

/// Tuple index for: Operate
#define STORM_POINT_OPERATE_OPERATE     0

/// Number of Tuples in the Point
#define STORM_POINT_OPERATE_NUM_TUPLES  (STORM_POINT_OPERATE_OPERATE+1)


/** RTE Point for the User operating mode, setpoints, fan mode, etc. a 
    thermostat.
 */
class Operate: public Rte::Point::Basic<STORM_POINT_OPERATE_NUM_TUPLES>
{
public: 
    /// Operate Tuple
    Storm::Tuple::Operate m_operate;


public:
    /// Constructor
    Operate( void )
        {
        registerTuple( STORM_POINT_OPERATE_OPERATE, m_operate );
        }

};


/*------------------------- MODEL POINTS ------------------------------------*/
/** Model Point for: Operate
 */
class OperateModel: public Operate,
                    public Rte::Point::Model::Base
{
public:
    /// Constructor
    OperateModel( Cpl::Itc::PostApi& myMbox )
        :Rte::Point::Model::Base(*this, myMbox)
            {
            }
};


/*------------------------- CONTROLLER POINTS -------------------------------*/
/** Controller Point: Operate
 */
class OperateController: public Operate,
                         public Rte::Point::Controller::Base
{
public:
    /// Constructor
    OperateController( OperateModel& modelPoint )
        :Rte::Point::Controller::Base(*this, modelPoint)
            {
            }

};


/** Read-Modify-Write Controller Point: Operate
 */
template <class CONTEXT>
class OperateRmwController: public Point::Operate,
                            public Rte::Point::Controller::RmwComposer<CONTEXT>
{
protected:
    /// Constructor.
    OperateRmwController( CONTEXT&                                                            context, 
                          typename Rte::Point::Controller::RmwComposer<CONTEXT>::ModifyFunc_T modifyCallback, 
                          OperateModel&                                                       modelPoint )
        :Rte::Point::Controller::RmwComposer<CONTEXT>(*this, context, modifyCallback, modelPoint )
            {}
};


/*------------------------- QUERY POINTS ------------------------------------*/
/** Query Point: Operate
 */
class OperateQuery: public Operate,
                    public Rte::Point::Query::Base
{
public:
    /// Constructor
    OperateQuery( OperateModel& modelPoint, bool initialAllInUseState=true, Rte::Point::Model::QueryRequest::Option_T copyOption = Rte::Point::Model::QueryRequest::eCOPY )
        :Rte::Point::Query::Base(*this, modelPoint, copyOption)
            {
            // Default to querying EVERYTHING
            setAllInUseState(initialAllInUseState);
            }

};


/*------------------------- VIEWER POINTS -----------------------------------*/
/** Viewer Point: Operate
 */
template <class CONTEXT>
class OperateViewer: public Operate,
                     public Rte::Point::Viewer::Composer<CONTEXT>
{
public:
    /// Constructor
    OperateViewer( CONTEXT&                                                                    context,
                   typename Rte::Point::Viewer::Composer<CONTEXT>::ChangeNotificationFunc_T    contextChangedCb,
                   typename Rte::Point::Viewer::Composer<CONTEXT>::StoppedNotificationFunc_T   contextStoppedCb,
                   OperateModel&                                                               modelPoint,
                   Cpl::Itc::PostApi&                                                          viewerMbox 
                 )
    :Rte::Point::Viewer::Composer<CONTEXT>::Composer(*this, context, contextChangedCb, contextStoppedCb, modelPoint, viewerMbox)
        {}
};


/** LIGHT WEIGHT Viewer Point: Operate
 */
template <class CONTEXT>
class OperateLViewer: public Rte::Point::Null<STORM_POINT_OPERATE_NUM_TUPLES>
                      public Rte::Point::Viewer::Composer<CONTEXT>
{
public:
    /// Constructor
    OperateLViewer( CONTEXT&                                                                    context,
                    typename Rte::Point::Viewer::Composer<CONTEXT>::ChangeNotificationFunc_T    contextChangedCb,
                    typename Rte::Point::Viewer::Composer<CONTEXT>::StoppedNotificationFunc_T   contextStoppedCb,
                    OperateModel&                                                               modelPoint,
                    Cpl::Itc::PostApi&                                                          viewerMbox 
                  )
    :Rte::Point::Viewer::Composer<CONTEXT>::Composer(*this, context, contextChangedCb, contextStoppedCb, modelPoint, viewerMbox)
        {}
};



};      // end namespace
};      // end namespace
#endif  // end header latch
