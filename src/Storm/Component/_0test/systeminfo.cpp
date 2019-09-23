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

#include "Catch/catch.hpp"
#include "Storm/Component/SystemInfo.h"
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Math/real.h"
#include "statics.h"


using namespace Storm::Component;

#define SECT_       "_0test"

////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "System Info" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();
    SystemInfo::Input_T  ins  = { mp_iduConfig, mp_oduConfig };
    SystemInfo::Output_T outs = { mp_allowedOperatingModes, mp_invalidConfigurationAlarm, mp_systemType, mp_systemForcedOffRefCnt };
    SystemInfo component( ins, outs );
    mp_systemForcedOffRefCnt.reset();

    // Start the component (and 'prime' it for the first real interval)
    Cpl::System::ElapsedTime::Precision_T time = { 0, 1 };
    component.start( time );
    component.doWork( true, time );
    time.m_thousandths += 1;

    SECTION( "combinations..." )
    {
        // NO valid configuration
        mp_iduConfig.setInvalid();
        mp_oduConfig.setInvalid();
        component.doWork( true, time );
        Storm::Type::AllowedOperatingModes value = Storm::Type::AllowedOperatingModes::eCOOLING_ONLY;
        int8_t valid = mp_allowedOperatingModes.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == false );
        Storm::Dm::MpSimpleAlarm::Data alarm;
        valid = mp_invalidConfigurationAlarm.read( alarm );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( alarm.active == true );
        Storm::Type::SystemType systemType = Storm::Type::SystemType::eUNDEFINED;
        valid = mp_systemType.read( systemType );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( systemType == +Storm::Type::SystemType::eUNDEFINED );
        uint32_t refCount;
        mp_systemForcedOffRefCnt.read( refCount );
        REQUIRE( refCount > 0 );

        // cooling with electric heat
        Storm::Dm::MpIduConfig::Data iduCfg = { Storm::Type::IduType::eAIR_HANDLER, true, 1 };
        Storm::Dm::MpOduConfig::Data oduCfg = { Storm::Type::OduType::eAC, 1 };
        mp_iduConfig.write( iduCfg );
        mp_oduConfig.write( oduCfg );
        time.m_thousandths += 1;
        component.doWork( true, time );
        value = Storm::Type::AllowedOperatingModes::eCOOLING_ONLY;
        valid = mp_allowedOperatingModes.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::AllowedOperatingModes::eCOOLING_AND_HEATING );
        mp_invalidConfigurationAlarm.read( alarm );
        REQUIRE( alarm.active == true );
        mp_systemType.read( systemType );
        REQUIRE( systemType == +Storm::Type::SystemType::eUNDEFINED );
        mp_systemForcedOffRefCnt.read( refCount );
        REQUIRE( refCount > 0 );


        // cooling only
        iduCfg = { Storm::Type::IduType::eAIR_HANDLER, true, 0 };
        oduCfg = { Storm::Type::OduType::eAC, 1 };
        mp_iduConfig.write( iduCfg );
        mp_oduConfig.write( oduCfg );
        time.m_thousandths += 1;
        component.doWork( true, time );
        valid = mp_allowedOperatingModes.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::AllowedOperatingModes::eCOOLING_ONLY );
        mp_invalidConfigurationAlarm.read( alarm );
        REQUIRE( alarm.active == false );
        mp_systemType.read( systemType );
        REQUIRE( systemType == +Storm::Type::SystemType::eAC1_AH0 );
        mp_systemForcedOffRefCnt.read( refCount );
        REQUIRE( refCount == 0 );

        // cooling with furnace heat
        iduCfg = { Storm::Type::IduType::eFURNACE, true, 1 };
        oduCfg = { Storm::Type::OduType::eAC, 1 };
        mp_iduConfig.write( iduCfg );
        mp_oduConfig.write( oduCfg );
        time.m_thousandths += 1;
        component.doWork( true, time );
        value = Storm::Type::AllowedOperatingModes::eCOOLING_ONLY;
        valid = mp_allowedOperatingModes.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::AllowedOperatingModes::eCOOLING_AND_HEATING );
        mp_invalidConfigurationAlarm.read( alarm );
        REQUIRE( alarm.active == false );
        mp_systemType.read( systemType );
        REQUIRE( systemType == +Storm::Type::SystemType::eAC1_FURN1 );
        mp_systemForcedOffRefCnt.read( refCount );
        REQUIRE( refCount == 0 );

        // Electric heat only
        iduCfg = { Storm::Type::IduType::eAIR_HANDLER, true, 1 };
        oduCfg = { Storm::Type::OduType::eAC, 0 };
        mp_iduConfig.write( iduCfg );
        mp_oduConfig.write( oduCfg );
        time.m_thousandths += 1;
        component.doWork( true, time );
        value = Storm::Type::AllowedOperatingModes::eCOOLING_ONLY;
        valid = mp_allowedOperatingModes.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::AllowedOperatingModes::eHEATING_ONLY );
        mp_invalidConfigurationAlarm.read( alarm );
        REQUIRE( alarm.active == true );
        mp_systemType.read( systemType );
        REQUIRE( systemType == +Storm::Type::SystemType::eUNDEFINED );
        mp_systemForcedOffRefCnt.read( refCount );
        REQUIRE( refCount > 0 );

        // Furnace heat only
        iduCfg = { Storm::Type::IduType::eFURNACE, true, 1 };
        oduCfg = { Storm::Type::OduType::eAC, 0 };
        mp_iduConfig.write( iduCfg );
        mp_oduConfig.write( oduCfg );
        time.m_thousandths += 1;
        component.doWork( true, time );
        value = Storm::Type::AllowedOperatingModes::eCOOLING_ONLY;
        valid = mp_allowedOperatingModes.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value == +Storm::Type::AllowedOperatingModes::eHEATING_ONLY );
        mp_invalidConfigurationAlarm.read( alarm );
        REQUIRE( alarm.active == false );
        mp_systemType.read( systemType );
        REQUIRE( systemType == +Storm::Type::SystemType::eNONE_FURN1 );
        mp_systemForcedOffRefCnt.read( refCount );
        REQUIRE( refCount == 0 );

        // No Capacity
        iduCfg = { Storm::Type::IduType::eFURNACE, true, 0 };
        oduCfg = { Storm::Type::OduType::eAC, 0 };
        mp_iduConfig.write( iduCfg );
        mp_oduConfig.write( oduCfg );
        time.m_thousandths += 1;
        component.doWork( true, time );
        value = Storm::Type::AllowedOperatingModes::eCOOLING_ONLY;
        valid = mp_allowedOperatingModes.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == false );
        mp_invalidConfigurationAlarm.read( alarm );
        REQUIRE( alarm.active == true );
        REQUIRE( alarm.critical == true );
        mp_systemType.read( systemType );
        REQUIRE( systemType == +Storm::Type::SystemType::eUNDEFINED );
        mp_systemForcedOffRefCnt.read( refCount );
        REQUIRE( refCount > 0 );
   }

    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}

