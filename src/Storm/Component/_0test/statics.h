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
#include "Storm/Dm/MpSetpoints.h"
#include "Storm/Dm/MpThermostatMode.h"
#include "Storm/Dm/MpOperatingMode.h"
#include "Cpl/Dm/Mp/Float.h"
#include "Cpl/Dm/Mp/Bool.h"
#include "Cpl/Dm/Mp/RefCounter.h"
#include "Cpl/Dm/Mp/ElapsedPrecisionTime.h"
#include <string.h>


// Help macros because I am OCD. The model point variable 
// name is prefixed with 'mp_'. For example:
// MP_INVALID(Cpl::Dm::Mp::Float, bob) == static Cpl::Dm::Mp::Float mp_bob;

// Creates model point in the invalid state. 
// Where t:=model Point type, n:=model point 'look-up' name, and 'mp_'+<n> is the model point variable name.
#define MP_INVALID(t, n)    static Cpl::Dm::StaticInfo info_mp_##n(#n); static t mp_##n(modelDb_, info_mp_##n ) 

////////////////////////////////////////////////////////////////////////////////

// Allocate/create my Model Database
static Cpl::Dm::ModelDatabase   modelDb_( "ignoreThisParameter_usedToInvokeTheStaticConstructor" );

// Allocate my Model Points
MP_INVALID( Storm::Dm::MpIdtAlarm, idtAlarms );
MP_INVALID( Cpl::Dm::Mp::Float, primaryRawIdt );
MP_INVALID( Cpl::Dm::Mp::Float, secondaryRawIdt );
MP_INVALID( Cpl::Dm::Mp::Float, activeIdt );
MP_INVALID( Cpl::Dm::Mp::Bool, enabledSecondaryIdt );
MP_INVALID( Cpl::Dm::Mp::RefCounter, systemForcedOffRefCnt );

MP_INVALID( Storm::Dm::MpSetpoints, setpoints );
MP_INVALID( Storm::Dm::MpOperatingMode, operatingMode );
MP_INVALID( Storm::Dm::MpThermostatMode, userMode );
MP_INVALID( Cpl::Dm::Mp::Bool, systemOn );
MP_INVALID( Cpl::Dm::Mp::Bool, operatingModeChanged );
MP_INVALID( Cpl::Dm::Mp::ElapsedPrecisionTime, beginOffTime );
MP_INVALID( Cpl::Dm::Mp::RefCounter, freezePiRefCnt );
MP_INVALID( Cpl::Dm::Mp::Bool, resetPiPulse );


