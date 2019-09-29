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
#include "Storm/Component/Control.h"
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Math/real.h"
#include "statics.h"


using namespace Storm::Component;

#define SECT_       "_0test"

class MyTestEquipment : public Control::Equipment
{
public:
    bool executeActive( Cpl::System::ElapsedTime::Precision_T  currentTick,
                        Cpl::System::ElapsedTime::Precision_T  currentInterval,
                        float                                  pvOut,
                        Storm::Type::SystemType                systemType,
                        Storm::Type::EquipmentTimes_T          equipmentBeginTimes,
                        Storm::Type::VirtualOutputs_T&         vOutputs,
                        Cpl::System::ElapsedTime::Precision_T& beginOnTime,
                        Cpl::System::ElapsedTime::Precision_T& beginOffTime,
                        bool&                                  systemOn ) noexcept
    {
        m_executeCount++;
        vOutputs.indoorFan     = m_executeCount * 2;
        beginOffTime.m_seconds = m_startCount * 10;
        beginOnTime.m_seconds  = m_operatingMode;
        systemOn               = m_executeCount & 0x1;    // Set to true when count is odd
        return true;
    }

    bool executeOff( Cpl::System::ElapsedTime::Precision_T  currentTick,
                     Cpl::System::ElapsedTime::Precision_T  currentInterval ) noexcept
    {
        m_executeOffCount += m_operatingMode;
        return true;
    }

    bool start( Cpl::System::ElapsedTime::Precision_T intervalTime ) noexcept
    {
        m_startCount++;
        return true;
    }

    Storm::Type::OperatingMode getOperatingMode() const noexcept
    {
        return Storm::Type::OperatingMode::_from_integral_unchecked( m_operatingMode );
    }

public:
    MyTestEquipment( Storm::Type::OperatingMode operatingMode ) : m_operatingMode( operatingMode ), m_executeCount( 0 ), m_executeOffCount(0), m_startCount( 0 ) {}

public:
    int m_operatingMode;
    int m_executeCount;
    int m_executeOffCount;
    int m_startCount;
};


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "Control" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();
    Control::Input_T  ins  = { mp_operatingMode, mp_pvOut, mp_systemType, mp_vOutputs };
    Control::Output_T outs = { mp_vOutputs, mp_beginOnTime, mp_beginOffTime, mp_systemOn };


    MyTestEquipment coolingEquipment( Storm::Type::OperatingMode::eCOOLING );
    MyTestEquipment heatingEquipment( Storm::Type::OperatingMode::eHEATING );
    Control componentCooling( coolingEquipment, ins, outs );
    Control componentHeating( heatingEquipment, ins, outs );

    // Start the components
    Cpl::System::ElapsedTime::Precision_T time = { 0, 1 };
    componentCooling.start( time );
    componentHeating.start( time );
    REQUIRE( coolingEquipment.m_startCount == 1 );
    REQUIRE( heatingEquipment.m_startCount == 1 );


    // Execute the Components in COOLING
    mp_systemType.write( Storm::Type::SystemType::eAC1_FURN1 );
    mp_pvOut.write( 0 );
    mp_operatingMode.write( Storm::Type::OperatingMode::eCOOLING );
    time.m_thousandths += 1;
    componentCooling.doWork( true, time );
    componentHeating.doWork( true, time );
    REQUIRE( coolingEquipment.m_startCount == 1 );
    REQUIRE( heatingEquipment.m_startCount == 1 );
    REQUIRE( coolingEquipment.m_executeCount == 1 );
    REQUIRE( heatingEquipment.m_executeCount == 0 );
    REQUIRE( coolingEquipment.m_executeOffCount == 0 );
    REQUIRE( heatingEquipment.m_executeOffCount == Storm::Type::OperatingMode::eHEATING );
    Storm::Type::VirtualOutputs_T         outputValues;
    Cpl::System::ElapsedTime::Precision_T beginOnValue;
    Cpl::System::ElapsedTime::Precision_T beginOffValue;
    bool     systemOnValue;
    mp_vOutputs.read( outputValues );
    mp_beginOnTime.read( beginOnValue );
    mp_beginOffTime.read( beginOffValue );
    mp_systemOn.read( systemOnValue );
    REQUIRE( outputValues.indoorFan == 1 * 2 );
    REQUIRE( beginOffValue.m_seconds == 1 * 10 );
    REQUIRE( beginOnValue.m_seconds == Storm::Type::OperatingMode::eCOOLING );
    REQUIRE( systemOnValue == true );


    // Execute the Components in HEATING
    mp_operatingMode.write( Storm::Type::OperatingMode::eHEATING );
    time.m_thousandths += 1;
    componentCooling.doWork( true, time );
    componentHeating.doWork( true, time );
    REQUIRE( coolingEquipment.m_startCount == 1 );
    REQUIRE( heatingEquipment.m_startCount == 1 );
    REQUIRE( coolingEquipment.m_executeCount == 1 );
    REQUIRE( heatingEquipment.m_executeCount == 1 );
    REQUIRE( coolingEquipment.m_executeOffCount == Storm::Type::OperatingMode::eCOOLING );
    REQUIRE( heatingEquipment.m_executeOffCount == Storm::Type::OperatingMode::eHEATING );
    mp_vOutputs.read( outputValues );
    mp_beginOnTime.read( beginOnValue );
    mp_beginOffTime.read( beginOffValue );
    mp_systemOn.read( systemOnValue );
    REQUIRE( outputValues.indoorFan == 1 * 2 );
    REQUIRE( beginOffValue.m_seconds == 1 * 10 );
    REQUIRE( beginOnValue.m_seconds == Storm::Type::OperatingMode::eHEATING );
    REQUIRE( systemOnValue == true );

    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}

