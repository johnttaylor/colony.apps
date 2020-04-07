#ifndef Storm_Thermostat_Persistence_UserRecord_h_
#define Storm_Thermostat_Persistence_UserRecord_h_
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

#include "colony_config.h"
#include "Cpl/Dm/Persistence/Record/Base.h"
#include "Cpl/Dm/SubscriberComposer.h"
#include "Cpl/Dm/Mp/Uint32.h"
#include "Storm/Dm/MpSetpoints.h"
#include "Storm/Dm/MpFanMode.h"
#include "Storm/Dm/MpThermostatMode.h"

/// Default value for the Indoor Air Filter timer
#ifndef OPTION_STORM_THERMOSTAT_PERSISTENCE_DEFAULT_AIR_FILTER_HOURS
#define OPTION_STORM_THERMOSTAT_PERSISTENCE_DEFAULT_AIR_FILTER_HOURS    360
#endif

/// Default thermostat/user mode
#ifndef OPTION_STORM_THERMOSTAT_PERSISTENCE_DEFAULT_USER_MODE
#define OPTION_STORM_THERMOSTAT_PERSISTENCE_DEFAULT_USER_MODE           Storm::Type::ThermostatMode::eOFF
#endif

/// Default fan mode
#ifndef OPTION_STORM_THERMOSTAT_PERSISTENCE_DEFAULT_FAN_MODE
#define OPTION_STORM_THERMOSTAT_PERSISTENCE_DEFAULT_FAN_MODE            Storm::Type::FanMode::eAUTO
#endif


///
namespace Storm {
///
namespace Thermostat {
///
namespace Persistence {


/** This concrete class implements the "Record" class for storing user settings.
 */
class UserRecord : public Cpl::Dm::Persistence::Record::Base
{
public:
    /// Constructor
    UserRecord( Cpl::Container::Map<Cpl::Dm::Persistence::Record::Api_>& recordList,
                Cpl::Dm::MailboxServer&                                  recordLayerMbox,
                unsigned long                                            delayWriteTimeInMsec = 2000 );


protected:
    /// See Cpl::Dm::Persistence::Record::Base
    void connectToModel() noexcept;

    /// See Cpl::Dm::Persistence::Record::Base
    void defaultData() noexcept;

    /// See Cpl::Dm::Persistence::Record::Base
    void disconnectFromModel() noexcept;


protected:
    /// Change notification
    void airFilterModelPointChanged( Cpl::Dm::Mp::Uint32& modelPointThatChanged ) noexcept;

    /// Change notification
    void fanModeModelPointChanged( Storm::Dm::MpFanMode& modelPointThatChanged ) noexcept;

    /// Change notification
    void userModeModelPointChanged( Storm::Dm::MpThermostatMode& modelPointThatChanged ) noexcept;

    /// Change notification
    void setpointModelPointChanged( Storm::Dm::MpSetpoints& modelPointThatChanged ) noexcept;

protected:
    ///
    Cpl::Dm::SubscriberComposer<UserRecord, Cpl::Dm::Mp::Uint32>         m_observerAirFilter;
    ///
    Cpl::Dm::SubscriberComposer<UserRecord, Storm::Dm::MpSetpoints>      m_observerSetpoints;
    ///
    Cpl::Dm::SubscriberComposer<UserRecord, Storm::Dm::MpThermostatMode> m_observerUserMode;
    ///
    Cpl::Dm::SubscriberComposer<UserRecord, Storm::Dm::MpFanMode>        m_observerFanMode;
    ///
    ModelPointReference_t                                                m_mpAirFilter;
    ///
    ModelPointReference_t                                                m_mpSetpoints;
    ///
    ModelPointReference_t                                                m_mpUserMode;
    ///
    ModelPointReference_t                                                m_mpFanMode;
};

};      // end namespace(s)
};
};

#endif  // end header latch
