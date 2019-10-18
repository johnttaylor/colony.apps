#ifndef Storm_Component_Equipment_State_BasicCooling_h_
#define Storm_Component_Equipment_State_BasicCooling_h_
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

#include "Storm/Component/Equipment/Stage.h"
#include "Storm/Component/Equipment/Stage/FsmEventQueue_.h"

/// Namespaces
namespace Storm {
/// Namespaces
namespace Component {
/// Namespaces
namespace Equipment {
/// Namespaces
namespace Stage {


/** This concrete class implements the Storm::Component::Equipment::Stage
    interface for a single compressor cooling stage, or multiple compressor
    cooling stages where transitions between stages have no restrictions other
    than the minimum cycle on/off times
 */
class BasicCooling : public Storm::Component::Equipment::Stage, public FsmEventQueue_
{
public:
    /// Constructor
    BasicCooling();

public:
    /// See Storm::Component::Equipment::Stage
    void reset() noexcept;

    /// See Storm::Component::Equipment::Stage
    void requestOn( Storm::Component::Control::Equipment::Args_T& args, bool startInOnCycle=true ) noexcept;

    /// See Storm::Component::Equipment::Stage
    void requestAsSupplement( Storm::Component::Control::Equipment::Args_T& args, Stage& nextStage, bool startNextStageInOnCycle=true ) noexcept;

    /// See Storm::Component::Equipment::Stage
    void requestOff( Storm::Component::Control::Equipment::Args_T& args, Stage* lowerStage=0, bool startLowerStageInOnCycle=true ) noexcept;

    /// See Storm::Component::Equipment::Stage
    void requestModeToOff( Storm::Component::Control::Equipment::Args_T& args ) noexcept;

    /// See Storm::Component::Equipment::Stage
    void execute( Storm::Component::Control::Equipment::Args_T& args ) noexcept;

public:
    /// See Storm::Component::Equipment::Stage
    bool isActive() const noexcept;

    /// See Storm::Component::Equipment::Stage
    bool isSupplementing() const noexcept;

    /// See Storm::Component::Equipment::Stage
    bool isOff() const noexcept;

    /// See Storm::Component::Equipment::Stage
    bool isTransitioningFromLowerStage() const noexcept;

    /// See Storm::Component::Equipment::Stage
    bool isTransitioningBackToLowerStage() const noexcept;

    /// See Storm::Component::Equipment::Stage
    bool isOnCycle() const noexcept;

    /// See Storm::Component::Equipment::Stage
    bool isOffCycle() const noexcept;

protected:
    /// See Storm::Component::Equipment::Stage
    void notifyAsActiveStage( Storm::Component::Control::Equipment::Args_T& args, bool startInOnCycle = true ) noexcept;

    /// See Storm::Component::Equipment::Stage
    void notifyAsExitingSupplmenting( Storm::Component::Control::Equipment::Args_T& args, bool startInOnCycle = true ) noexcept;


public:
    /// Action
    void EnterSupplementing() noexcept;

    /// Action
    void ExitSupplementing() noexcept;

    /// Action
    void checkBackTransition() noexcept;

    /// Action
    void checkFromTransition() noexcept;

    /// Action
    void checkOffTime() noexcept;

    /// Action
    void checkOnTime() noexcept;

    /// Action
    void checkStartingOffTime() noexcept;

    /// Action
    void checkStartingOnTime() noexcept;

    /// Action
    void initializeActive() noexcept;

    /// Action
    void initializeBackTransition() noexcept;

    /// Action
    void initializeFromTransition() noexcept;

    /// Action
    void notifyLowerStage() noexcept;

    /// Action
    void shutdownStage() noexcept;

    /// Action
    void startCyclingInOffCycle() noexcept;

    /// Action
    void startCyclingInOnCycle() noexcept;

    /// Action
    void startOffTime() noexcept;

    /// Action
    void startOnTime() noexcept;

    /// Action
    void startStageOff() noexcept;

    /// Action
    void startStageOn() noexcept;


public:
    /// Guard
    bool initializeStage() noexcept;

    /// Guard
    bool isBeingSupplemented() noexcept;

    /// Guard
    bool isStartInOffCycle() noexcept;


};


};      // end namespace
};
};
};
#endif  // end header latch
