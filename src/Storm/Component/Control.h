#ifndef Storm_Component_Control_h_
#define Storm_Component_Control_h_
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

#include "Storm/Component/Base.h"
#include "Cpl/Dm/Mp/ElapsedPrecisionTime.h"
#include "Cpl/Dm/Mp/Float.h"
#include "Cpl/Dm/Mp/Bool.h"
#include "Storm/Dm/MpOperatingMode.h"
#include "Storm/Dm/MpSystemType.h"
#include "Storm/Dm/MpVirtualIduOutputs.h"
#include "Storm/Dm/MpVirtualOduOutputs.h"

/// Namespaces
namespace Storm
{
/// Namespaces
namespace Component
{
/** This concrete class provides the Component wrapper that is responsible for
    determining how to control the HVAC equipment to meet the current load and/or
    user inputs.  The concrete Equipment object (provide as a constructor
    argument) implements the actual control business logic.

 */
class Control : public Base
{
public:

    /** This abstract class defines the interface for the Control Component to
        invoke the control logic/processing on HVAC system with a specific equipment
        configuration and/or operating mode (i.e. heating or cooling)
     */
    class Equipment
    {
    public:
        /** This method will be called on a periodic basis (as determined by the
            calling Control Component instance) to perform active conditioning.
            This method will only be called when the current operating mode 'matches'
            the operating configuration of the Equipment instance.

            The method returns true if no error(s) occurred; else false is returned.
            Note: If false is return, then the Component's MP HVAC outputs will be
                  updated to an "all off state" (the Outdoor SOV output will
                  not be changed if/when this happens).
         */
        virtual bool execute( Cpl::System::ElapsedTime::Precision_T  currentTick,
                              Cpl::System::ElapsedTime::Precision_T  currentInterval,
                              float                                  pvOut,
                              Storm::Type::SystemType                systemType,
                              Storm::Dm::MpVirtualIduOutputs::Data&  vIduOutputs,
                              Storm::Dm::MpVirtualOduOutputs::Data&  vOduOutputs,
                              Cpl::System::ElapsedTime::Precision_T& beginOnTime,
                              Cpl::System::ElapsedTime::Precision_T& beginOffTime,
                              bool&                                  systemOn ) noexcept = 0;

        /** This method completes any/all initialization for the Equipment
            instance.  The method is called when the containing Component instance's
            start() method is called. This method return true when successfully;
            else false is returned.
         */
        virtual bool start( Cpl::System::ElapsedTime::Precision_T intervalTime ) noexcept = 0;

        /** Returns the Equipment instance's operating mode
         */
        virtual Storm::Type::OperatingMode getOperatingMode() const noexcept = 0;

    public:
        /// Virtual destructor
        virtual ~Equipment() {}
    };

public:
    /// Input Model Points
    struct Input_T
    {
        Storm::Dm::MpOperatingMode&     operatingMode;          //!< The actual operating thermostat mode (derived from the User mode setting)
        Cpl::Dm::Mp::Float&             pvOut;                  //!< Output of the PI Controller.  This is unit-less positive number that ranges from 0.0 to piConstants.maxPvOut
        Storm::Dm::MpSystemType&        systemType;             //!< The current system configuration/type based on the current indoor/outdoor equipment settings
    };


    /// Output Model Points
    struct Output_T
    {
        Storm::Dm::MpVirtualIduOutputs&     vIduOutputs;            //!< The virtual Indoor unit outputs
        Storm::Dm::MpVirtualOduOutputs&     vOduOutputs;            //!< The virtual Outdoor unit outputs
        Cpl::Dm::Mp::ElapsedPrecisionTime&  beginOnTime;            //!< The starting time of the current On cycle
        Cpl::Dm::Mp::ElapsedPrecisionTime&  beginOffTime;           //!< The starting time of the current Off cycle
        Cpl::Dm::Mp::Bool&                  systemOn;               //!< This flag is true the equipment/system is considered to be in the "on state"
    };


public:
    /// Constructor
    Control( Equipment& controlLogic, struct Input_T ins, struct Output_T outs );

    /// See Storm::Component::Api
    bool start( Cpl::System::ElapsedTime::Precision_T intervalTime );

protected:
    /// See Storm::Component::Base
    bool execute( Cpl::System::ElapsedTime::Precision_T currentTick,
                  Cpl::System::ElapsedTime::Precision_T currentInterval );

protected:
    /// My Model Point inputs
    struct Input_T  m_in;

    /// My Model Point outputs
    struct Output_T m_out;

    /// The Control logic
    Equipment& m_equipment;
};



};      // end namespace
};      // end namespace
#endif  // end header latch
