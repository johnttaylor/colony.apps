#ifndef Storm_Rte_Point_UserConfig_h_
#define Storm_Rte_Point_UserConfig_h_
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

#include "Storm/Rte/Tuple/UserConfig.h"
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

#define STORM_RTE_POINT_USER_CONFIG_CONFIG          0   //!< Tuple Index
#define STORM_RTE_POINT_USER_CONFIG_NUM_TUPLES      (STORM_RTE_POINT_USER_CONFIG_CONFIG+1)  //!< Number of Tuples


/** RTE Point for the User operating mode, setpoints, fan mode, etc. a 
    thermostat.
 */
class UserConfig: public Rte::Point::Basic<STORM_RTE_POINT_USER_CONFIG_NUM_TUPLES>
{
public: 
    /// UserConfig Tuple
    Storm::Tuple::UserConfig m_config;


public:
    /// Constructor
    UserConfig( void )
        {
        registerTuple( STORM_RTE_POINT_USER_CONFIG_CONFIG, m_config );
        }

};


/*------------------------- MODEL POINTS ------------------------------------*/
/** Model Point for: UserConfig
 */
class UserConfigModel: public UserConfig,
                       public Rte::Point::Model::Base
{
public:
    /// Constructor
    UserConfigModel( Cpl::Itc::PostApi& myMbox )
        :Rte::Point::Model::Base(*this, myMbox)
            {
            }
};


/*------------------------- CONTROLLER POINTS -------------------------------*/
/** Controller Point: UserConfig
 */
class UserConfigController: public UserConfig,
                            public Rte::Point::Controller::Base
{
public:
    /// Constructor
    UserConfigController( UserConfigModel& modelPoint )
        :Rte::Point::Controller::Base(*this, modelPoint)
            {
            }

};


/** Tuple Controller Point: UserConfig
 */
class UserConfigTupleController: public UserConfig
                                 public Rte::Point::Controller::Tuple
{
public:
    /// Constructor
    UserConfigTupleController( UserConfigModel& modelPoint, unsigned myTupleItemIdx = 0 )
        :Rte::Point::Controller::Tuple(myTupleItemIdx, *this, modelPoint)
            {
            }

};


/** Read-Modify-Write Controller Point: UserConfig
 */
template <class CONTEXT>
class UserConfigRmwController: public Point::UserConfig,
                               public Rte::Point::Controller::RmwComposer<CONTEXT>
{
protected:
    /// Constructor.
    UserConfigRmwController( CONTEXT&                                                            context, 
                             typename Rte::Point::Controller::RmwComposer<CONTEXT>::ModifyFunc_T modifyCallback, 
                             UserConfigModel&                                                    modelPoint 
                           )
        :Rte::Point::Controller::RmwComposer<CONTEXT>(*this, context, modifyCallback, modelPoint )
            {}
};


/*------------------------- QUERY POINTS ------------------------------------*/
/** Query Point: UserConfig
 */
class UserConfigQuery: public UserConfig,
                       public Rte::Point::Query::Base
{
public:
    /// Constructor
    UserConfigQuery( UserConfigModel& modelPoint, bool initialAllInUseState=true, Rte::Point::Model::QueryRequest::Option_T copyOption = Rte::Point::Model::QueryRequest::eCOPY )
        :Rte::Point::Query::Base(*this, modelPoint, copyOption)
            {
            // Default to querying EVERYTHING
            setAllInUseState(initialAllInUseState);
            }

};

/** Tuple Query Point: UserConfig (Single Tuple, no traversal)
 */
class UserConfigQueryTuple: public UserConfig, 
                            public Rte::Point::Query::Tuple
{
public:
    /// Constructor
    UserConfigQueryTuple( UserConfigModel&                          modelPoint, 
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
/** Viewer Point: UserConfig
 */
template <class CONTEXT>
class UserConfigViewer: public UserConfig,
                        public Rte::Point::Viewer::Composer<CONTEXT>
{
public:
    /// Constructor
    UserConfigViewer( CONTEXT&                                                                    context,
                      typename Rte::Point::Viewer::Composer<CONTEXT>::ChangeNotificationFunc_T    contextChangedCb,
                      typename Rte::Point::Viewer::Composer<CONTEXT>::StoppedNotificationFunc_T   contextStoppedCb,
                      UserConfigModel&                                                            modelPoint,
                      Cpl::Itc::PostApi&                                                          viewerMbox 
                    )
    :Rte::Point::Viewer::Composer<CONTEXT>::Composer(*this, context, contextChangedCb, contextStoppedCb, modelPoint, viewerMbox)
        {}
};


/** LIGHT WEIGHT Viewer Point: UserConfig
 */
template <class CONTEXT>
class UserConfigLViewer: public Rte::Point::Null<STORM_RTE_POINT_USER_CONFIG_NUM_TUPLES>
                         public Rte::Point::Viewer::Composer<CONTEXT>
{
public:
    /// Constructor
    UserConfigLViewer( CONTEXT&                                                                    context,
                       typename Rte::Point::Viewer::Composer<CONTEXT>::ChangeNotificationFunc_T    contextChangedCb,
                       typename Rte::Point::Viewer::Composer<CONTEXT>::StoppedNotificationFunc_T   contextStoppedCb,
                       UserConfigModel&                                                            modelPoint,
                       Cpl::Itc::PostApi&                                                          viewerMbox 
                     )
    :Rte::Point::Viewer::Composer<CONTEXT>::Composer(*this, context, contextChangedCb, contextStoppedCb, modelPoint, viewerMbox)
        {}
};



};      // end namespace
};      
};      
#endif  // end header latch
