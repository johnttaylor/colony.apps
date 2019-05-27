#ifndef Storm_Type_Point_InstallerConfig_h_
#define Storm_Type_Point_InstallerConfig_h_
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

#include "Storm/Type/Tuple/PiConfig.h"
#include "Storm/Type/Tuple/EquipConfig.h"
#include "Storm/Type/Tuple/StageConfig.h"
#include "Rte/Point/Basic.h"
#include "Rte/Point/Model/Base.h"
#include "Rte/Point/Controller/Base.h"
#include "Rte/Point/Controller/RmwComposer.h"
#include "Rte/Point/Controller/Tuple.h"
#include "Rte/Point/Query/Base.h"
#include "Rte/Point/Query/Tuple.h"
#include "Rte/Point/Viewer/Composer.h"


/// Namespaces
namespace Storm
    {
    namespace Type
        {
        namespace Point
            {


            /*------------------------- POINTS ------------------------------------------*/

            #define STORM_TYPE_POINT_INSTALLERCONFIG_PI_CONFIG           0  //!< Tuple Index
            #define STORM_TYPE_POINT_INSTALLERCONFIG_EQUIP_CONFIG        1  //!< Tuple Index
            #define STORM_TYPE_POINT_INSTALLERCONFIG_COMP_COOL_STAGE1    2  //!< Tuple Index
            #define STORM_TYPE_POINT_INSTALLERCONFIG_COMP_COOL_STAGE2    3  //!< Tuple Index
            #define STORM_TYPE_POINT_INSTALLERCONFIG_COMP_HEAT_STAGE1    4  //!< Tuple Index
            #define STORM_TYPE_POINT_INSTALLERCONFIG_COMP_HEAT_STAGE2    5  //!< Tuple Index
            #define STORM_TYPE_POINT_INSTALLERCONFIG_ELEC_HEAT_STAGE1    6  //!< Tuple Index
            #define STORM_TYPE_POINT_INSTALLERCONFIG_ELEC_HEAT_STAGE2    7  //!< Tuple Index
            #define STORM_TYPE_POINT_INSTALLERCONFIG_ELEC_HEAT_STAGE3    8  //!< Tuple Index
            #define STORM_TYPE_POINT_INSTALLERCONFIG_FOSSIL_HEAT_STAGE1  9  //!< Tuple Index
            #define STORM_TYPE_POINT_INSTALLERCONFIG_FOSSIL_HEAT_STAGE2  10 //!< Tuple Index
            #define STORM_TYPE_POINT_INSTALLERCONFIG_FOSSIL_HEAT_STAGE3  11 //!< Tuple Index
            #define STORM_TYPE_POINT_INSTALLERCONFIG_NUM_TUPLES          (Storm_Type_Point_INSTALLERCONFIG_FOSSIL_HEAT_STAGE3+1)  //!< Number of Tuples


            /** RTE Point for the Installer Configuration Operations
             */
            class InstallerConfig : public Rte::Point::Basic<STORM_TYPE_POINT_INSTALLERCONFIG_NUM_TUPLES>
                {
                public:
                    Storm::Rte::Tuple::PiConfig      m_piConfig;         //!< Tuple
                    Storm::Rte::Tuple::EquipConfig   m_equipConfig;      //!< Tuple
                    Storm::Rte::Tuple::StageConfig   m_compCoolStage1;   //!< Tuple
                    Storm::Rte::Tuple::StageConfig   m_compCoolStage2;   //!< Tuple
                    Storm::Rte::Tuple::StageConfig   m_compHeatStage1;   //!< Tuple
                    Storm::Rte::Tuple::StageConfig   m_compHeatStage2;   //!< Tuple
                    Storm::Rte::Tuple::StageConfig   m_elecHeatStage1;   //!< Tuple
                    Storm::Rte::Tuple::StageConfig   m_elecHeatStage2;   //!< Tuple
                    Storm::Rte::Tuple::StageConfig   m_elecHeatStage3;   //!< Tuple
                    Storm::Rte::Tuple::StageConfig   m_fossilHeatStage1; //!< Tuple
                    Storm::Rte::Tuple::StageConfig   m_fossilHeatStage2; //!< Tuple
                    Storm::Rte::Tuple::StageConfig   m_fossilHeatStage3; //!< Tuple


                public:
                    /// Constructor
                    InstallerConfig( void )
                        {
                        registerTuple( Storm_Type_Point_INSTALLERCONFIG_PI_CONFIG, m_piConfig );
                        registerTuple( Storm_Type_Point_INSTALLERCONFIG_EQUIP_CONFIG, m_equipConfig );
                        registerTuple( Storm_Type_Point_INSTALLERCONFIG_COMP_COOL_STAGE1, m_compCoolStage1 );
                        registerTuple( Storm_Type_Point_INSTALLERCONFIG_COMP_COOL_STAGE2, m_compCoolStage2 );
                        registerTuple( Storm_Type_Point_INSTALLERCONFIG_COMP_HEAT_STAGE1, m_compHeatStage1 );
                        registerTuple( Storm_Type_Point_INSTALLERCONFIG_COMP_HEAT_STAGE2, m_compHeatStage2 );
                        registerTuple( Storm_Type_Point_INSTALLERCONFIG_ELEC_HEAT_STAGE1, m_elecHeatStage1 );
                        registerTuple( Storm_Type_Point_INSTALLERCONFIG_ELEC_HEAT_STAGE2, m_elecHeatStage2 );
                        registerTuple( Storm_Type_Point_INSTALLERCONFIG_ELEC_HEAT_STAGE3, m_elecHeatStage3 );
                        registerTuple( Storm_Type_Point_INSTALLERCONFIG_FOSSIL_HEAT_STAGE1, m_fossilHeatStage1 );
                        registerTuple( Storm_Type_Point_INSTALLERCONFIG_FOSSIL_HEAT_STAGE2, m_fossilHeatStage2 );
                        registerTuple( Storm_Type_Point_INSTALLERCONFIG_FOSSIL_HEAT_STAGE3, m_fossilHeatStage3 );
                        }

                };


            /*------------------------- MODEL POINTS ------------------------------------*/
            /** Model Point for: InstallerConfig
             */
            class InstallerConfigModel : public InstallerConfig,
                public Rte::Point::Model::Base
                {
                public:
                    /// Constructor
                    InstallerConfigModel( Cpl::Itc::PostApi& myMbox )
                        : Rte::Point::Model::Base( *this, myMbox )
                        {
                        }
                };


            /*------------------------- CONTROLLER POINTS -------------------------------*/
            /** Controller Point: InstallerConfig
             */
            class InstallerConfigController : public InstallerConfig,
                public Rte::Point::Controller::Base
                {
                public:
                    /// Constructor
                    InstallerConfigController( InstallerConfigModel& modelPoint )
                        : Rte::Point::Controller::Base( *this, modelPoint )
                        {
                        }

                };


            /** Tuple Controller Point: InstallerConfig
             */
             //class InstallerConfigTupleController: public InstallerConfig,
             //                                      public Rte::Point::Controller::Tuple
             //{
             //public:
             //    /// Constructor
             //    InstallerConfigTupleController( InstallerConfigModel& modelPoint, unsigned myTupleItemIdx = 0 )
             //        : Rte::Point::Controller::Tuple(myTupleItemIdx, *this, modelPoint)
             //            {
             //            }

             //};


             /** Read-Modify-Write Controller Point: InstallerConfig
              */
            template <class CONTEXT>
            class InstallerConfigRmwController : public Point::InstallerConfig,
                public Rte::Point::Controller::RmwComposer<CONTEXT>
                {
                protected:
                    /// Constructor.
                    InstallerConfigRmwController( CONTEXT&                                                              context,
                                                  typename Rte::Point::Controller::RmwComposer<CONTEXT>::ModifyFunc_T modifyCallback,
                                                  InstallerConfigModel&                                                 modelPoint
                                           )
                        : Rte::Point::Controller::RmwComposer<CONTEXT>( *this, context, modifyCallback, modelPoint )
                        {}
                };


            /*------------------------- QUERY POINTS ------------------------------------*/
            /** Query Point: InstallerConfig
             */
            class InstallerConfigQuery : public InstallerConfig,
                public Rte::Point::Query::Base
                {
                public:
                    /// Constructor
                    InstallerConfigQuery( InstallerConfigModel& modelPoint, bool initialAllInUseState=true, Rte::Point::Model::QueryRequest::Option_T copyOption = Rte::Point::Model::QueryRequest::eCOPY )
                        : Rte::Point::Query::Base( *this, modelPoint, copyOption )
                        {
                        // Default to querying EVERYTHING
                        setAllInUseState( initialAllInUseState );
                        }

                };

            /** Tuple Query Point: InstallerConfig (Single Tuple, no traversal)
             */
             //class InstallerConfigQueryTuple: public InstallerConfig, 
             //                                 public Rte::Point::Query::Tuple
             //{
             //public:
             //    /// Constructor
             //    InstallerConfigQueryTuple( InstallerConfigModel&                       modelPoint, 
             //                               unsigned                                    tupleIndex = 0, 
             //                               Rte::Point::Model::QueryRequest::Option_T copyOption = Rte::Point::Model::QueryRequest::eCOPY 
             //                        )
             //        : Rte::Point::Query::Tuple(tupleIndex, *this, modelPoint, copyOption )
             //            {
             //            // Default to querying EVERYTHING
             //            setAllInUseState(true);
             //            }

             //};



             /*------------------------- VIEWER POINTS -----------------------------------*/
             /** Viewer Point: InstallerConfig
              */
            template <class CONTEXT>
            class InstallerConfigViewer : public InstallerConfig,
                public Rte::Point::Viewer::Composer<CONTEXT>
                {
                public:
                    /// Constructor
                    InstallerConfigViewer( CONTEXT&                                                                      context,
                                           typename Rte::Point::Viewer::Composer<CONTEXT>::ChangeNotificationFunc_T    contextChangedCb,
                                           typename Rte::Point::Viewer::Composer<CONTEXT>::StoppedNotificationFunc_T   contextStoppedCb,
                                           InstallerConfigModel&                                                         modelPoint,
                                           Cpl::Itc::PostApi&                                                            viewerMbox
                                         )
                        : Rte::Point::Viewer::Composer<CONTEXT>::Composer( *this, context, contextChangedCb, contextStoppedCb, modelPoint, viewerMbox )
                        {}
                };


            /** LIGHT WEIGHT Viewer Point: InstallerConfig
             */
            template <class CONTEXT>
            class InstallerConfigLViewer : public Rte::Point::Null<STORM_TYPE_POINT_INSTALLERCONFIG_NUM_TUPLES>,
                public Rte::Point::Viewer::Composer<CONTEXT>
                {
                public:
                    /// Constructor
                    InstallerConfigLViewer( CONTEXT&                                                                      context,
                                            typename Rte::Point::Viewer::Composer<CONTEXT>::ChangeNotificationFunc_T    contextChangedCb,
                                            typename Rte::Point::Viewer::Composer<CONTEXT>::StoppedNotificationFunc_T   contextStoppedCb,
                                            InstallerConfigModel&                                                         modelPoint,
                                            Cpl::Itc::PostApi&                                                            viewerMbox
                                     )
                        : Rte::Point::Viewer::Composer<CONTEXT>::Composer( *this, context, contextChangedCb, contextStoppedCb, modelPoint, viewerMbox )
                        {}
                };



            };      // end namespace
        };
    };
#endif  // end header latch
