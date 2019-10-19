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

#include "Storm/Component/Equipment/Stage/Basic.h"
#include "Storm/Component/PiPreProcess.h"

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
    cooling stages where transitions between stages have no restrictions that
    the Stage object needs-to/is-required to support (i.e. all stage transition 
    logic is handled by the Equipment instance).
 */
class BasicCooling : public Basic
{
public:
    /// Constructor.  Defaults to a single stage cooling
    BasicCooling( float pvLowerBound = 0.0F, float pvUpperBound = OPTION_STORM_COMPONENT_PI_PREPROCESS_COOLING_LV_PER_STAGE, unsigned comfortStageIndex = 0, unsigned outdoorStageIndex = 0);

public:
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
    void enterSupplementing() noexcept;

    /// Action
    void exitSupplementing() noexcept;

    /// Action
    void initializeActive() noexcept;

    /// Action.
    void initializeBackTransition() noexcept;

    /// Action
    void initializeFromTransition() noexcept;

    /// Action
    void shutdownStage() noexcept;

    /// Action
    void stageOff() noexcept;

    /// Action
    void stageOn() noexcept;

    /// Action
    void startCyclingInOffCycle() noexcept;

    /// Action
    void startCyclingInOnCycle() noexcept;

    /// Action
    void startingStageOff() noexcept;

    /// Action
    void startingStageOn() noexcept;

protected:
    /// PV (aka Load) lower bound for the stage
    float m_pvLowerBound;

    /// PV (aka Load) upper bound for the stage
    float m_pvUpperBound;

    /// Index for the stage's Comfort Control configuration data
    unsigned m_ccIndex;

    /// Index for the stage's Outdoor unit stage HVAC output
    unsigned m_outIndex;

};


};      // end namespace
};
};
};
#endif  // end header latch
