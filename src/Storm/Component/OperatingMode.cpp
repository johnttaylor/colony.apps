/*-----------------------------------------------------------------------------
* This file is part of the Colony.Apps Project.  The Colony.Apps Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.apps/license.txt
*
* Copyright (c) 2015 John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/


#include "OperatingMode.h"
#include "Cpl/System/Trace.h"


#define SECT_                   "Storm::Component::OperatingMode"


/// Namespaces
using namespace Storm::Component;


///////////////////////////////
OperatingMode::OperatingMode( Rte::Element::Float&                    i_coolingSetpoint,
                              Rte::Element::Float&                    i_heatingSetpoint,
                              Storm::Type::Element::TMode&            i_userMode,
                              Rte::Element::Float&                    i_idt,
                              Rte::Element::Integer32&                i_freezePiRefCount,
                              Rte::Element::ElapsedPrecisionTime&     i_beginOffTime,
                              Storm::Type::Element::Pulse&            i_resetPi,
                              Rte::Element::Boolean&                  i_systemOn,
                              Rte::Element::Integer32&                o_freezePiRefCount,
                              Storm::Type::Element::OMode&            o_opMode,
                              Storm::Type::Element::Pulse&            o_resetPi,
                              Storm::Type::Element::Pulse&            o_opModeChanged
                            )
    : mi_coolingSetpoint( i_coolingSetpoint )
    , mi_heatingSetpoint( i_heatingSetpoint )
    , mi_userMode( i_userMode )
    , mi_idt( i_idt )
    , mi_freezePiRefCount( i_freezePiRefCount )
    , mi_beginOffTime( i_beginOffTime )
    , mi_resetPi( i_resetPi )
    , mi_systemOn( i_systemOn )
    , mo_freezePiRefCount( o_freezePiRefCount )
    , mo_opMode( o_opMode )
    , mo_resetPi( o_resetPi )
    , mo_opModeChanged( o_opModeChanged )
    {
    }



bool OperatingMode::start( Cpl::System::ElapsedTime::Precision_T intervalTime )
    {
    // Initialize my data
    m_prevOperatingMode  = Storm::Type::Element::OMode_T::eINVALID;  // Start with an invalid mode since I don't know what my mode is/should be!
    m_inAuto             = false;

    // Initialize parent class
    return Base::start( intervalTime );
    }



///////////////////////////////
bool OperatingMode::execute( Cpl::System::ElapsedTime::Precision_T currentTick,
                             Cpl::System::ElapsedTime::Precision_T currentInterval
                           )
    {
    CPL_SYSTEM_TRACE_FUNC( SECT_ );

    //--------------------------------------------------------------------------
    // Pre-Algorithm processing
    //--------------------------------------------------------------------------

    // Default the output values
    mo_freezePiRefCount.set( mi_freezePiRefCount.get() );
    mo_resetPi.set( mi_resetPi.get() );
    mo_opMode.set( m_prevOperatingMode );
    mo_opModeChanged.reset();


    //--------------------------------------------------------------------------
    // Algorithm processing
    //--------------------------------------------------------------------------

    // Trap broken IDT sensor
    if (!mi_idt.isValid())
        {
        setNewOMode( Storm::Type::Element::OMode_T::eOFF );
        }

    // Convert the User Thermostat mode to Operating mode
    else
        {
        switch (mi_userMode.get())
            {
            // COOLING
                case Storm::Type::Element::TMode_T::eCOOLING:
                    m_inAuto = false;
                    setNewOMode( Storm::Type::Element::OMode_T::eCOOLING );
                    break;


                    // HEATING
                case Storm::Type::Element::TMode_T::eHEATING:
                    m_inAuto = false;
                    setNewOMode( Storm::Type::Element::OMode_T::eHEATING );
                    break;


                    // Resovle AUTO mode
                case Storm::Type::Element::TMode_T::eAUTO:
                    // Trap first time through
                    if (!m_inAuto)
                        {
                        m_inAuto = true;
                        if (mi_idt.get() <= mi_heatingSetpoint.get())
                            {
                            setNewOMode( Storm::Type::Element::OMode_T::eHEATING );
                            }
                        else
                            {
                            setNewOMode( Storm::Type::Element::OMode_T::eCOOLING );
                            }
                        }

                    // Nominal path
                    else
                        {
                        static const Cpl::System::ElapsedTime::Precision_T timeHysteresis ={ OPTION_STORM_COMPONENT_OPERATING_MODE_SECONDS_HYSTERESIS, 0 };

                        // Only switch modes if the system has been off for at least N seconds.
                        if (mi_systemOn.get() == false && Cpl::System::ElapsedTime::expiredPrecision( mi_beginOffTime.get(), timeHysteresis, currentInterval ))
                            {
                            if (mi_idt.get() >= mi_coolingSetpoint.get() - OPTION_STORM_COMPONENT_OPERATING_MODE_COOLING_OFFSET)
                                {
                                setNewOMode( Storm::Type::Element::OMode_T::eCOOLING );
                                }
                            else
                                {
                                setNewOMode( Storm::Type::Element::OMode_T::eHEATING );
                                }
                            }
                        }
                    break;


                    // OFF mode (and any invalid mode settings)
                default:
                    m_inAuto = false;
                    setNewOMode( Storm::Type::Element::OMode_T::eOFF );
                    break;
            }
        }


    // If I get here -->everything worked!
    return true;
    }

    /////////////////
    void Storm::Component::OperatingMode::setNewOMode( Storm::Type::Element::OMode_T::Enum newOMode )
    {
    // React to a change in the operating mode
    if (newOMode != m_prevOperatingMode)
        {
        // Set indication that the Operating mode is/was changed
        mo_opModeChanged.pulse();

        // Reset PI on mode changes
        mo_resetPi.pulse();

        // Manage freeze/unfreeze the PI when transition in/out of the OFF state
        if (newOMode == Storm::Type::Element::OMode_T::eOFF)
            {
            mo_freezePiRefCount.set( mo_freezePiRefCount.get() + 1);
            }
        else if (m_prevOperatingMode == Storm::Type::Element::OMode_T::eOFF)
            {
            mo_freezePiRefCount.set( mo_freezePiRefCount.get() - 1 );
            }

        // Set the new mode and cache it       
        mo_opMode.set( m_prevOperatingMode = newOMode );
        }
    }

