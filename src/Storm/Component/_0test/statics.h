/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2019  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "Cpl/Dm/ModelDatabase.h"
#include "Storm/Dm/MpIdtAlarm.h"
#include "Cpl/Dm/Mp/Float.h"
#include "Cpl/Dm/Mp/Bool.h"
#include "Cpl/Dm/Mp/RefCounter.h"
#include <string.h>



////////////////////////////////////////////////////////////////////////////////

// Allocate/create my Model Database
static Cpl::Dm::ModelDatabase   modelDb_( "ignoreThisParameter_usedToInvokeTheStaticConstructor" );

// Allocate my Model Points
static Cpl::Dm::StaticInfo      info_idtAlarms_( "IdtAlarms" );
static Storm::Dm::MpIdtAlarm    mp_idtAlarms( modelDb_, info_idtAlarms_ );

static Cpl::Dm::StaticInfo      info_mp_primaryRawIdt_( "primaryRawIdt" );
static Cpl::Dm::Mp::Float       mp_primaryRawIdt_( modelDb_, info_mp_primaryRawIdt_ );
static Cpl::Dm::StaticInfo      info_mp_secondaryRawIdt_( "secondaryRawIdt" );
static Cpl::Dm::Mp::Float       mp_secondaryRawIdt_( modelDb_, info_mp_secondaryRawIdt_ );
static Cpl::Dm::StaticInfo      info_mp_activeIdt_( "activeIdt" );
static Cpl::Dm::Mp::Float       mp_activeIdt_( modelDb_, info_mp_activeIdt_ );

static Cpl::Dm::StaticInfo      info_mp_haveSecondaryIdt_( "haveSecondaryIdt" );
static Cpl::Dm::Mp::Bool        mp_haveSecondaryIdt_( modelDb_, info_mp_haveSecondaryIdt_ );

static Cpl::Dm::StaticInfo      info_mp_systemForcedOff_( "systemForcedOff" );
static Cpl::Dm::Mp::RefCounter  mp_systemForcedOff_( modelDb_, info_mp_systemForcedOff_ );

