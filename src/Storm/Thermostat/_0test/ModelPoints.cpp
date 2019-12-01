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

#include "ModelPoints.h"
#include "Storm/Thermostat/ModelPoints.h" 

// Help macros because I am OCD. The model point variable 
// name is prefixed with 'mp_'. For example:
// MP_INVALID(Cpl::Dm::Mp::Float, bob) == static Cpl::Dm::Mp::Float mp_bob;

// Creates model point in the invalid state. 
// Where t:=model Point type, n:=model point 'look-up' name, and 'mp_'+<n> is the model point variable name.
#define MP_INVALID(t, n)    static Cpl::Dm::StaticInfo info_mp_##n(#n); t mp_##n(g_modelDatabase, info_mp_##n ) 

////////////////////////////////////////////////////////////////////////////////

// Allocate/create my Model Database
// NOTE: For the MickySoft compiler I must 'allocate' the g_modelDatabase before any
//       model points get instantiated.  By placing the allocation in the _0test 
//       directory AND by using nqbp's 'firstObjects' feature (and setting the _0test
//       directory to be a 'firstObjects') it seems to appease the MS gods.
Cpl::Dm::ModelDatabase   g_modelDatabase( "ignoreThisParameter_usedToInvokeTheStaticConstructor" );

// Allocate my Model Points
MP_INVALID( Cpl::Dm::Mp::Bool, houseSimEnabled );
MP_INVALID( Cpl::Dm::Mp::Double, outdoorTemp );
