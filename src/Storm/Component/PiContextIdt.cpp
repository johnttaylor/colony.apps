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


#include "PiContextIdt.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Math/real.h"


#define SECT_               "Storm::Component::PiContextIdt"

#define INVALID_SETPOINT_   9999.0



/// Namespaces
using namespace Storm::Component;


///////////////////////////////
PiContextIdt::PiContextIdt( void )
    {
    }


void PiContextIdt::initialize( void )
    {
    m_activeSetpoint = INVALID_SETPOINT_;
    }


///////////////////////////////
void PiContextIdt::execute( Cpl::System::ElaspedTime::Precision_T currentTick, 
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
    outputs.m_deltaError    = 0.0;
    outputs.m_deltaSetpoint = 0.0;
    outputs.m_setpointChanged.reset();


    // Use the current operating mode to calculate the delta error
    float newActiveSetpoint = INVALID_SETPOINT_;
    switch( m_opMode )
        {
        // COOLING
        case Storm::Type::OMode::eCOOLING:
            outputs.m_deltaError = inputs.m_idt - cfg.m_coolingSetpoint;
            newActiveSetpoint    = cfg.m_coolingSetpoint;
            break;

        // HEATING
        case Storm::Type::OMode::eHEATING:
            outputs.m_deltaError = cfg.m_heatingSetpoint - inputs.m_idt;
            newActiveSetpoint    = cfg.m_heatingSetpoint;
            break;

        // OFF mode (and any invalid mode settings)
        default:
            m_activeSetpoint = INVALID_SETPOINT_;
            break;
        }


    // Trap a change in the active setpoint (but NOT when there is a mode change)
    if ( inputs.m_opModeChanged.isPulsed() == false )
        {
        outputs.m_deltaSetpoint = fabs( newActiveSetpoint - m_activeSetpoint );
        if ( Cpl::Math::areFloatsEqual( outputs.m_deltaSetpoint, 0.0 ) == false )   
            {
            outputs.m_setpointChanged.pulse();
            }
        }

    // Update my cached active setpoint
    m_activeSetpoint = newActiveSetpoint;


    // All done -->set the outputs
    if ( !setOutputs( &outputs ) )
        {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "[%p] Failed setOutputs", this ) );
        }
    }


