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

#define SECT_   "Storm::Component::OperatingMode"


/// Namespaces
using namespace Storm::Component;



///////////////////////////////
OperatingMode::OperatingMode( void )
    {
    }


void OperatingMode::initialize( void )
    {
    m_prevOperatingMode = Storm::Type::OMode::eINVALID;  // Start with an invalid mode since I don't know what my mode is/should be!
    m_inAuto            = false;
    }


///////////////////////////////
void OperatingMode::execute( Cpl::System::ElaspedTime::Precision_T currentTick, 
                             Cpl::System::ElaspedTime::Precision_T currentInterval 
                           )
    {
    CPL_SYSTEM_TRACE_FUNC( SECT_ );

    // Get Config & Inputs
    Configuration_T cfg;
    Input_T         inputs;
    if ( !getConfiguration( &cfg ) )
        {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "[%p] Failed getConfiguration", this ) );
        return;
        }
    if ( !getInputs( &inputs ) )
        {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "[%p] Failed getInputs", this ) );
        return;
        }


    // Default the output values
    Output_T outputs;
    outputs.m_freezePiRefCount = inputs.m_freezePiRefCount;
    outputs.m_opMode           = m_prevOperatingMode;
    outputs.m_resetPi          = inputs.m_resetPi;
    outputs.m_opModeChanged.reset();



    // Convert the User Thermostat mode to Operating mode
    switch( cfg.m_userMode )
        {
        // COOLING
        case Storm::Type::TMode::eCOOLING:
            m_inAuto = false;
            setNewOMode( Storm::Type::OMode::eCOOLING );
            break;


        // HEATING
        case Storm::Type::TMode::eHEATING:
            m_inAuto = false;
            setNewOMode( Storm::Type::OMode::eHEATING );
            break;


        // Resovle AUTO mode
        case Storm::Type::TMode::eAUTO;
            // Trap first time through
            if ( !m_inAuto )
                {
                m_inAuto = true;
                setNewOMode( inputs.m_idt <= cfg.m_heatingSetpoint? Storm::Type::OMode::eHEATING: Storm::Type::TMode::eCOOLING  );
                }

            // Nominal path
            else
                {
                static const Cpl::System::ElaspedTime::Precision_T timeHysteresis = { OPTION_STORM_COMPONENT_OPERATING_MODE_SECONDS_HYSTERESIS, 0 };

                // Only switch modes if the system has been off for at least N seconds.
                if ( inputs.m_systemOn == false && Cpl::System::ElaspedTime::expiredPrecision( inputs.m_beginOffTime, timeHysteresis, currentInterval ) )
                    {
                    if ( inputs.m_idt >= cfg.m_coolingSetpoint - OPTION_STORM_COMPONENT_OPERATING_MODE_COOLING_OFFSET )
                        {
                        setNewOMode( Storm::Type::OMode::eCOOLING );
                        }
                    else
                        {
                        setNewOMode( Storm::Type::OMode::eHEATING );
                        }
                    }
                }                      
            break;

    
        // OFF mode (and any invalid mode settings)
        default:
            m_inAuto = false;
            setNewOMode( Storm::Type::OMode::eOFF );
            break;
        }


    // All done -->set the outputs
    if ( !setOutputs( &outputs ) )
        {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "[%p] Failed setOutputs", this ) );
        }
    }


///////////////////////////////
void OperatingMode::setNewOMode( Storm::Type::OMode::Enum_t newOMode, Ouput_T& outputs )
    {
    // Only act on a change of mode
    if ( newOMode != m_prevOperatingMode )
        {
        // Set indication that the Operating mode is/was changed
        outputs.m_opModeChanged.pulse();

        // Reset PI on mode changes
        outputs.m_resetPi.pulse();

        // Manage freeze/unfreeze the PI when in the OFF state
        if ( newOMode == Storm::Type::TMode::eOFF )
            {
            outputs.m_freezePiRefCount++;
            }
        else if ( m_prevOperatingMode == Storm::Type::TMode::eOFF )
            {
            outputs.m_freezePiRefCount--;
            }
            
        // Set the new mode        
        outputs.m_opMode = newOMode;
        }
    }

