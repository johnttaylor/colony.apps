#ifndef Storm_Component_Equipment_Cooling_h_
#define Storm_Component_Equipment_Cooling_h_
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

#include "Storm/Component/Control.h"
#include "Storm/Component/Equipment/Stage.h"


/** Process Variable (PV) turn-on threshold for cooling operation
 */
#ifndef OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_ON_THRESHOLD  
#define OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_ON_THRESHOLD              5.0F
#endif

/** Process Variable (PV) turn-off threshold for cooling operation
 */
#ifndef OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_OFF_THRESHOLD  
#define OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_OFF_THRESHOLD             0.0F
#endif


/** Process Variable (PV) turn-on threshold for 2nd stage cooling operation
 */
#ifndef OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_ON_2ND_STAGE_THRESHOLD  
#define OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_ON_2ND_STAGE_THRESHOLD    110.0F
#endif

/** Process Variable (PV) turn-off threshold for 2nd stage cooling operation
 */
#ifndef OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_OFF_2ND_STAGE_THRESHOLD  
#define OPTION_STORM_COMPONENT_EQUIPMENT_COOLING_TURN_OFF_2ND_STAGE_THRESHOLD   100.0F
#endif

///
namespace Storm
{
///
namespace Component
{
///
namespace Equipment
{
/** This concrete class provides the business logic for controlling the HVAC
    system when the system is in the Cooling Mode.  Specifically this class
    contain the logic how to use/manage the N stages of cooling capacity
 */
class Cooling : public Storm::Component::Control::Equipment
{
public:
    /// Constructor. TODO: ADD Support 2 stage cooling
    Cooling( Stage& firstStageCooling /*, Stage& secondStageCooling */ );


public:
    /// See Storm::Component::Control::Equipment
    bool executeActive( Storm::Type::SystemType systemType, Args_T& args ) noexcept;

    /// See Storm::Component::Control::Equipment
    bool executeOff( Storm::Type::SystemType systemType, Args_T& args ) noexcept;

    /// See Storm::Component::Control::Equipment
    bool start( Cpl::System::ElapsedTime::Precision_T intervalTime ) noexcept;
    
    /// See Storm::Component::Control::Equipment
    void reset() noexcept;

    /// See Storm::Component::Control::Equipment
    Storm::Type::OperatingMode getOperatingMode() const noexcept { return Storm::Type::OperatingMode::eOFF; }


protected:
    /// Control Single Staged Cooling equipment.  Returns true if 'action was taken'
    bool singleStage( Storm::Type::SystemType systemType, Args_T& args ) noexcept;

    // TODO: ADD Support 2 stage cooling
    // Control Two Staged Cooling equipment. Returns true if 'action was taken'
    //bool secondStage( Storm::Type::SystemType systemType, Args_T& args ) noexcept;


protected:
    /// Reference to Single Stage cooling logic
    Stage&  m_1StageCooling;

    /// Reference to Two Stage cooling logic
    //Stage&  m_2StageCooling;

};



};      // end namespace
};
};
#endif  // end header latch