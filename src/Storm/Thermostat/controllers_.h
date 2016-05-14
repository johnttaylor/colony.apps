#ifndef Storm_Thermostat_controllers_h_
#define Storm_Thermostat_controllers_h_
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

#include "Storm/Thermostat/DataDictionary.h"
#include "Rte/Point/Controller/Base.h"
#include "Rte/Point/Controller/RmwComposer.h"
#include "Rte/Point/Controller/Tuple.h"


/// Namespaces
namespace Storm { namespace Thermostat {



/*------------------------- CONTROLLER POINTS -------------------------------*/
/** Controller Point: DataDictionary
 */
class DataDictionaryController: public DataDictionary,
                                public ::Rte::Point::Controller::Base
{
public:
    /// Constructor
    DataDictionaryController( DataDictionaryModel& modelPoint )
        : ::Rte::Point::Controller::Base(*this, modelPoint)
            {
            }

};


/** Tuple Controller Point: DataDictionary
 */
//class DataDictionaryTupleController: public DataDictionary,
//                                     public ::Rte::Point::Controller::Tuple
//{
//public:
//    /// Constructor
//    DataDictionaryTupleController( DataDictionaryModel& modelPoint, unsigned myTupleItemIdx = 0 )
//        : ::Rte::Point::Controller::Tuple(myTupleItemIdx, *this, modelPoint)
//            {
//            }

//};


/** Read-Modify-Write Controller Point: DataDictionary
 */

class DataDictionaryRmwController: public DataDictionary,
                                   public ::Rte::Point::Controller::RmwComposer<CONTEXT>
{
protected:
    /// Constructor.
    DataDictionaryRmwController( CONTEXT&                                                              context, 
                                 typename ::Rte::Point::Controller::RmwComposer<CONTEXT>::ModifyFunc_T modifyCallback, 
                                 DataDictionaryModel&                                                  modelPoint 
                               )
        : ::Rte::Point::Controller::RmwComposer<CONTEXT>(*this, context, modifyCallback, modelPoint )
            {}
};




};      // end namespace
};      
#endif  // end header latch
