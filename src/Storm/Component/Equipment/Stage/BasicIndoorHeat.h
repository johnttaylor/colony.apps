#ifndef Storm_Component_Equipment_State_BasicIndoorHeat_h_
#define Storm_Component_Equipment_State_BasicIndoorHeat_h_
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
class BasicIndoorHeat : public Basic
{
public:
    /// Constructor.  Defaults to a single stage cooling
    BasicIndoorHeat( float    pvLowerBound      = 0.0F,
                     float    pvUpperBound      = OPTION_STORM_COMPONENT_PI_PREPROCESS_HEATING_LV_PER_STAGE,
                     unsigned comfortStageIndex = 0,
                     unsigned indoorStageIndex  = 0,
                     unsigned stageIndex        = 0,
                     bool     controlIndoorFan  = false  /* default to Furnace operation */ );

    /** Used to 'configure' stage after it has been constructed.
       
        Note: This method should ONLY be called when the stage is in the 'off' state
     */
    void configure( float pvLowerBound, float pvUpperBound, unsigned comfortStageIndex, unsigned indoorStageIndex, unsigned stageIndex, bool controlIndoorFan );

public:
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
    /// Index for the stage's Comfort Control configuration data
    unsigned m_ccIndex;

    /// Index for the stage's Outdoor unit stage HVAC output
    unsigned m_outIndex;

    /// Zero based index of the stage, e.g. the first indoor heating stage is index 0, second indoor heating stage is index 1, etc.
    unsigned m_stageIndex;

    /// Set to true when the Fan should be actively controller (a.k.a. when indoor heat:= Electric Heat)
    bool     m_controlFan;

};


};      // end namespace
};
};
};
#endif  // end header latch
