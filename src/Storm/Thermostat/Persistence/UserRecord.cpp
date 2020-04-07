/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2020  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/

#include "UserRecord.h"
#include "Storm/Thermostat/ModelPoints.h"

using namespace Storm::Thermostat::Persistence;



//////////////////////
UserRecord::UserRecord( Cpl::Container::Map<Cpl::Dm::Persistence::Record::Api_>& recordList,
                        Cpl::Dm::MailboxServer&                                  recordLayerMbox,
                        unsigned long                                            delayWriteTimeInMsec )
    : Base( recordList, delayWriteTimeInMsec, "UserCfg", recordLayerMbox )
    , m_observerAirFilter( recordLayerMbox, *this, &UserRecord::airFilterModelPointChanged )
    , m_observerSetpoints( recordLayerMbox, *this, &UserRecord::setpointModelPointChanged )
    , m_observerUserMode( recordLayerMbox, *this, &UserRecord::userModeModelPointChanged )
    , m_observerFanMode( recordLayerMbox, *this, &UserRecord::fanModeModelPointChanged )
    , m_mpAirFilter( mp_maxAirFilterHours )
    , m_mpSetpoints( mp_setpoints )
    , m_mpUserMode( mp_userMode )
    , m_mpFanMode( mp_fanMode )
{
    registerModelPoint( m_mpAirFilter );
    registerModelPoint( m_mpSetpoints );
    registerModelPoint( m_mpUserMode );
    registerModelPoint( m_mpFanMode );
}


//////////////////////
void UserRecord::connectToModel() noexcept
{
    mp_maxAirFilterHours.attach( m_observerAirFilter, mp_maxAirFilterHours.getSequenceNumber() );
    mp_setpoints.attach( m_observerSetpoints, mp_setpoints.getSequenceNumber() );
    mp_userMode.attach( m_observerUserMode, mp_userMode.getSequenceNumber() );
    mp_fanMode.attach( m_observerFanMode, mp_fanMode.getSequenceNumber() );
}

void UserRecord::disconnectFromModel() noexcept
{
    mp_maxAirFilterHours.detach( m_observerAirFilter );
    mp_setpoints.detach( m_observerSetpoints );
    mp_userMode.detach( m_observerUserMode );
    mp_fanMode.detach( m_observerFanMode );
}

void UserRecord::defaultData() noexcept
{
    mp_maxAirFilterHours.write( OPTION_STORM_THERMOSTAT_PERSISTENCE_DEFAULT_AIR_FILTER_HOURS );
    mp_setpoints.write( OPTION_STORM_DM_MP_SETPOINTS_DEFAULT_COOLING, OPTION_STORM_DM_MP_SETPOINTS_DEFAULT_HEATING );
    mp_userMode.write( OPTION_STORM_THERMOSTAT_PERSISTENCE_DEFAULT_USER_MODE );
    mp_fanMode.write( OPTION_STORM_THERMOSTAT_PERSISTENCE_DEFAULT_FAN_MODE );
}


//////////////////////
void UserRecord::airFilterModelPointChanged( Cpl::Dm::Mp::Uint32& modelPointThatChanged ) noexcept
{
    generateEvent( Cpl::Dm::Persistence::Record::Fsm_evDataModified );
}

void UserRecord::fanModeModelPointChanged( Storm::Dm::MpFanMode& modelPointThatChanged ) noexcept
{
    generateEvent( Cpl::Dm::Persistence::Record::Fsm_evDataModified );
}

void UserRecord::userModeModelPointChanged( Storm::Dm::MpThermostatMode& modelPointThatChanged ) noexcept
{
    generateEvent( Cpl::Dm::Persistence::Record::Fsm_evDataModified );
}

void UserRecord::setpointModelPointChanged( Storm::Dm::MpSetpoints& modelPointThatChanged ) noexcept
{
    generateEvent( Cpl::Dm::Persistence::Record::Fsm_evDataModified );
}

