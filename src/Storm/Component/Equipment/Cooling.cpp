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

#include "Cooling.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Math/real.h"


#define SECT_ "Storm::Component::Equipment"

using namespace Storm::Component::Equipment;

//////////////////////////////////////////////////////////////
Cooling::Cooling( Stage& singleStageCooling /*, Stage& twoStageCooling */ )
    : m_1StageCooling( singleStageCooling )
    // , m_2StageCooling( twoStageCooling ) // TODO: ADD Support 2 stage cooling
{
}

//////////////////////////////////////////////////////////////
bool Cooling::executeActive( Storm::Type::SystemType systemType, Args_T& args ) noexcept
{
    switch ( systemType )
    {
    case Storm::Type::SystemType::eAC1_AH0:
    case Storm::Type::SystemType::eAC1_FURN1:
        // Single Stage logic
        singleStage( systemType, args );

        // 'Run' the stage(s)
        m_1StageCooling.execute( args );
        break;

        // TODO: ADD Support 2 stage cooling
        //case Storm::Type::SystemType::eAC2_AH0:
        //case Storm::Type::SystemType::eAC2_FURN1:
        //    // 2nd Stage logic
        //    if ( !secondStage( args ) )
        //    {
        //        // 1st Stage logic
        //        singleStage( args );
        //    }

        //    // 'Run' the stage(s)
        //    m_1StageCooling.execute( args );
        //    m_2StageCooling.execute( args );
        //    break;

    default:
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Cooling::executeActive: Unsupported SystemType (%d)", systemType ) );
        return false;
    }

    return true;
}

bool Cooling::executeOff( Storm::Type::SystemType systemType, Args_T& args ) noexcept
{
    switch ( systemType )
    {
    case Storm::Type::SystemType::eAC1_AH0:
    case Storm::Type::SystemType::eAC1_FURN1:
        m_1StageCooling.requestModeToOff( args );
        break;

        // TODO: ADD Support 2 stage cooling
        //case Storm::Type::SystemType::eAC2_AH0:
        //case Storm::Type::SystemType::eAC2_FURN1:
        //    m_1StageCooling.requestModeToOff( args );
        //    m_2StageCooling.requestModeToOff( args );
        //    break;

    default:
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "Cooling::executeOff: Unsupported SystemType (%d)", systemType ) );
        return false;
    }

    return true;
}

bool Cooling::start( Cpl::System::ElapsedTime::Precision_T intervalTime ) noexcept
{
    // initialize Stage object(s)
    m_1StageCooling.reset();

    // TODO: ADD Support 2 stage cooling
    //m_2StageCooling.reset();
}
void Cooling::reset() noexcept
{
    // initialize Stage object(s)
    m_1StageCooling.reset();

    // TODO: ADD Support 2 stage cooling
    //m_2StageCooling.reset();
}

//////////////////////////////////////////////////////////////
bool Cooling::singleStage( Storm::Type::SystemType systemType, Args_T& args ) noexcept
{
    // If the system was previously off AND there is sufficient load to turn on
    if ( args.systemOn == false && args.pvOut > OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_ON_THRESHOLD )
    {
        // Has the system met the minimum compressor off time?
        Cpl::System::ElapsedTime::Precision_T minOffTime = { OPTION_STORM_MIN_COMPRESSOR_OFF_TIME_SEC, 0 };
        if ( Cpl::System::ElapsedTime::expiredPrecision( args.equipmentBeginTimes.outdoorUnitBeginOffTime, minOffTime, args.currentInterval ) )
        {
            m_1StageCooling.requestOn( args );
        }

        return true;
    }

    // stage 1 is the active stage AND there is no longer sufficient load requiring conditioning 
    else if ( m_1StageCooling.isActive() && args.pvOut <= OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_OFF_THRESHOLD)
    {
        // Am I in an off-cycle OR have the system met the minimum 1st stage on cycle time?
        Cpl::System::ElapsedTime::Precision_T minOnTime = { args.comfortConfig.cooling[0].minOnTime, 0 };
        if ( m_1StageCooling.isOffCycle() || Cpl::System::ElapsedTime::expiredPrecision( args.cycleInfo.beginOnTime, minOnTime, args.currentInterval ) )
        {
            m_1StageCooling.requestOff( args );
        }

        return true;
    }

    // Return false because no 'logic was tripped'
    return false;
}

// TODO: ADD Support 2 stage cooling
//bool Cooling::secondStage( Storm::Type::SystemType systemType, Args_T& args ) noexcept
//{
//    // If the system was previously off AND there is sufficient load to turn on WITH THE SECOND STAGE
//    if ( args.systemOn == false && args.pvOut > OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_ON_2ND_STAGE_THRESHOLD )
//    {
//        // Has the system met the minimum compressor off time?
//        Cpl::System::ElapsedTime::Precision_T minOffTime = { OPTION_STORM_MIN_COMPRESSOR_OFF_TIME_SEC, 0 };
//        if ( Cpl::System::ElapsedTime::expiredPrecision( args.equipmentBeginTimes.outdoorUnitBeginOffTime, minOffTime, args.currentInterval ) )
//        {
//            m_1StageCooling.requestOn( args );
//            m_1StageCooling.requestAsSupplement( args, m_2StageCooling );
//        }
//
//        return true;
//    }
//
//    // cycling the 1st stage AND there is sufficient load to turn on with the SECOND stage
//    else if ( m_1StageCooling.isActive() && args.pvOut > OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_ON_2ND_STAGE_THRESHOLD )
//    {
//        // Has the system met the minimum 1st stage equipment on time?
//        Cpl::System::ElapsedTime::Precision_T minOnTime = { OPTION_STORM_MIN_COMPRESSOR_STAGE_1OF2_ON_TIME_SEC, 0 };
//        if ( Cpl::System::ElapsedTime::expiredPrecision( args.equipmentBeginTimes.outdoorUnitBeginOnTime, minOnTime, args.currentInterval ) )
//        {
//            m_1StageCooling.requestAsSupplement( args, m_2StageCooling );
//        }
//
//        return true;
//    }
//
//    // Cycling the 2nd stage AND there is no longer sufficient load for the 2nd stage
//    else if ( m_2StageCooling.isActive() && args.pvOut <= OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_OFF_THRESHOLD )
//    {
//        // Am I in an off-cycle OR have met the minimum 2nd stage on cycle time?
//        Cpl::System::ElapsedTime::Precision_T minOnTime = { args.comfortConfig.cooling[1].minOnTime, 0 };
//        if ( m_1StageCooling.isOffCycle() || Cpl::System::ElapsedTime::expiredPrecision( args.cycleInfo.beginOnTime, minOnTime, args.currentInterval ) )
//        {
//            m_2StageCooling.requestOff( args, &m_1StageCooling );
//        }
//
//        return true;
//    }
// 
//    // Return false because no 'logic was tripped'
//    return false;
//}