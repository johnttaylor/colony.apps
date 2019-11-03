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
    BasicCooling( float    pvLowerBound      = 0.0F,
                  float    pvUpperBound      = OPTION_STORM_COMPONENT_PI_PREPROCESS_COOLING_LV_PER_STAGE,
                  unsigned comfortStageIndex = 0,
                  unsigned outdoorStageIndex = 0 );

public:
    /// Action
    void initializeActive() noexcept;

    /// Action
    void stageOff() noexcept;

    /// Action
    void stageOn() noexcept;

protected:
    /// See Basic Api
    uint32_t getOffCycleMinTime( Storm::Component::Control::Equipment::Args_T& args ) const noexcept;

    /// See Basic Api
    uint32_t getOnCycleMinTime( Storm::Component::Control::Equipment::Args_T& args ) const noexcept;

    /// See Basic Api
    Storm::Type::Cph getCycleCph( Storm::Component::Control::Equipment::Args_T& args ) const noexcept;


protected:
};


};      // end namespace
};
};
};
#endif  // end header latch
