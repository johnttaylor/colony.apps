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


bool PiContextIdt::start( Cpl::System::ElaspedTime::Precision_T intervalTime )
    {
    // Initialize my data
    m_prevActiveSetpoint = INVALID_SETPOINT_;

    // Initialize parent class
    return Base::start( intervalTime );
    }


///////////////////////////////
bool PiContextIdt::execute( Cpl::System::ElaspedTime::Precision_T currentTick, 
                            Cpl::System::ElaspedTime::Precision_T currentInterval 
                          )
    {
    CPL_SYSTEM_TRACE_FUNC( SECT_ );

    //--------------------------------------------------------------------------
    // Pre-Algorithm processing   
    //--------------------------------------------------------------------------

    // Get Inputs
    Input_T  inputs;
    if ( !getInputs( inputs ) )
        {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "[%p] Failed getInputs", this ) );
        return false;
        }

    // Default the output values
    Output_T outputs;
    outputs.m_deltaError    = 0.0;
    outputs.m_deltaSetpoint = 0.0;
    outputs.m_gain          = 0.0;
    outputs.m_resetTime     = 3.4f;

    outputs.m_maxOutValue   = 0.0;
    outputs.m_setpointChanged.reset();


    //--------------------------------------------------------------------------
    // Algorithm processing
    //--------------------------------------------------------------------------

    // Use the current operating mode to calculate the delta error
    float newActiveSetpoint;
    switch(inputs.m_opMode )
        {
        // COOLING
        case Storm::Type::OMode::eCOOLING:
            newActiveSetpoint     = inputs.m_coolingSetpoint;
            outputs.m_deltaError  = inputs.m_idt - inputs.m_coolingSetpoint;
            outputs.m_maxOutValue = calcPiCoolingMaxOut( inputs );
            if (inputs.m_coolingFastPiEnabled )
                {
                outputs.m_gain        = inputs.m_coolingGain1;
                outputs.m_resetTime   = inputs.m_coolingResetTime1;
                }
            else
                {
                outputs.m_gain        = inputs.m_coolingGain0;
                outputs.m_resetTime   = inputs.m_coolingResetTime0;
                }
            break;


        // HEATING
        case Storm::Type::OMode::eHEATING:
            newActiveSetpoint     = inputs.m_heatingSetpoint;
            outputs.m_deltaError  = inputs.m_heatingSetpoint - inputs.m_idt;
            outputs.m_maxOutValue = calcPiHeatingMaxOut( inputs );
            if (inputs.m_heatingFastPiEnabled )
                {
                outputs.m_gain        = inputs.m_heatingGain1;
                outputs.m_resetTime   = inputs.m_heatingResetTime1;
                }
            else
                {
                outputs.m_gain        = inputs.m_heatingGain0;
                outputs.m_resetTime   = inputs.m_heatingResetTime0;
                }
            break;


        // OFF mode (and any invalid mode settings)
        default:
            newActiveSetpoint = INVALID_SETPOINT_;
            break;
        }


    // Trap a change in the active setpoint (but NOT when there is a mode change)
    if ( inputs.m_opModeChanged.isPulsed() == false )
        {
        outputs.m_deltaSetpoint = fabs( newActiveSetpoint - m_prevActiveSetpoint );
        if ( Cpl::Math::areFloatsEqual( outputs.m_deltaSetpoint, 0.0 ) == false )   
            {
            outputs.m_setpointChanged.pulse();
            }
        }

    // Update my cached active setpoint
    m_prevActiveSetpoint = newActiveSetpoint;


    //--------------------------------------------------------------------------
    // Post-Algorithm processing
    //--------------------------------------------------------------------------

    // All done -->set the outputs
    if ( !setOutputs( outputs ) )
        {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "[%p] Failed setOutputs", this ) );
        return false;
        }

    return true;
    }




///////////////////////////////
float PiContextIdt::calcPiCoolingMaxOut( Input_T& inputs )
    {
    return inputs.m_coolingNumStages * OPTION_STORM_COMPONENT_PI_CONTEXT_IDT_COOLING_LV_PER_STAGE;
    }
     
float PiContextIdt::calcPiHeatingMaxOut( Input_T& inputs )
    {
    float primary = inputs.m_noPrimaryHeat? 0.0f: inputs.m_heatingNumPriStages * OPTION_STORM_COMPONENT_PI_CONTEXT_IDT_HEATING_LV_PER_STAGE;
    return primary + (inputs.m_heatingNumSecStages * OPTION_STORM_COMPONENT_PI_CONTEXT_IDT_HEATING_LV_PER_STAGE);
    }
