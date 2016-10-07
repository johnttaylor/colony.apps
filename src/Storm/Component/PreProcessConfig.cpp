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


#include "PreProcessConfig.h"
#include "Cpl/System/Trace.h"


#define SECT_               "Storm::Component::PreProcessConfig"




/// Namespaces
using namespace Storm::Component;


///////////////////////////////
PreProcessConfig::PreProcessConfig( void )
    {
    }


bool PreProcessConfig::start( Cpl::System::ElaspedTime::Precision_T intervalTime )
    {
    // Initialize parent class
    return Base::start( intervalTime );
    }


///////////////////////////////
bool PreProcessConfig::execute( Cpl::System::ElaspedTime::Precision_T currentTick, 
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
    outputs.m_heatingNumPriStages = 0;
    outputs.m_heatingNumSecStages = 0;


    //--------------------------------------------------------------------------
    // Algorithm processing
    //--------------------------------------------------------------------------

    outputs.m_heatingNumPriStages = getNumHeatStages( inputs.m_primaryHeatSource,   inputs );
    outputs.m_heatingNumPriStages = getNumHeatStages( inputs.m_secondaryHeatSource, inputs );


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
unsigned PreProcessConfig::getNumHeatStages( Storm::Type::HeatType::Enum_T source, Input_T& inputs )
    {
    unsigned result = 0;

    switch( source )
        {
            case Storm::Type::HeatType::eFOSSIL:
            result = inputs.m_numFossilHeatingStages;
            break;

        case Storm::Type::HeatType::eELECTRIC:
            result = inputs.m_numElecHeatingStages;
            break;

        case Storm::Type::HeatType::eMECHANICAL:
            result = inputs.m_numCompHeatingStages;
            break;

        default:
            break;
        }

    return result;
    }

