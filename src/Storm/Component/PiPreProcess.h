#ifndef Storm_Component_PiPreProcess_h_
#define Storm_Component_PiPreProcess_h_
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

#include "colony_config.h"
#include "Storm/Component/Base.h"
#include "Storm/Dm/MpIduConfig.h"
#include "Storm/Dm/MpOduConfig.h"
#include "Storm/Dm/MpOperatingMode.h"
#include "Storm/Dm/MpSetpoints.h"
#include "Storm/Dm/MpPiConstants.h"
#include "Cpl/Dm/Mp/Float.h"
#include "Cpl/Dm/Mp/Bool.h"

/** Magic constant the represent the 'amount' LV per cooling stage
 */
#ifndef OPTION_STORM_COMPONENT_PI_PREPROCESS_COOLING_LV_PER_STAGE
#define OPTION_STORM_COMPONENT_PI_PREPROCESS_COOLING_LV_PER_STAGE     100.0F
#endif


 /** Magic constant the represent the 'amount' LV per heating stage
  */
#ifndef OPTION_STORM_COMPONENT_PI_PREPROCESS_HEATING_LV_PER_STAGE
#define OPTION_STORM_COMPONENT_PI_PREPROCESS_HEATING_LV_PER_STAGE     100.0F
#endif

/// Namespace
namespace Storm {
/// Namespace
namespace Component {


/** This concrete class provide the context for a PI controller.  The
    'context' is responsible defining the current delta error input, PI gains
    & reset times, max OUT, etc. to the PI controller.
 */
class PiPreProcess : public Base
{
public:
    /// Input Model Points
    struct Input_T
    {
        Cpl::Dm::Mp::Float&             activeIdt;              //!< The current indoor temperature (in degrees F)
        Storm::Dm::MpOperatingMode&     operatingMode;          //!< The current operating mode
        Cpl::Dm::Mp::Bool&              operatingModeChange;    //!< When true, indicates that the current operating mode (heating vs cooling) has changed
        Storm::Dm::MpSetpoints          setpoints;              //!< The current heating/cooling setpoints (in degrees F)
        Storm::Dm::MpIduConfig&         iduConfig;              //!< Indoor Unit configuration
        Storm::Dm::MpOduConfig&         oduConfig;              //!< Outdoor Unit configuration
    };


    /// Output Model Points
    struct Output_T
    {
        Cpl::Dm::Mp::Float&             activeSetpoint;         //!< The active setpoint (based on the current operating mode)
        Cpl::Dm::Mp::Float&             idtDeltaError;          //!< The delta error (in degrees F) between the current IDT the 'active' setpoint
        Cpl::Dm::Mp::Float&             setpointDelta;          //!< The change (in degrees F) in the 'active' setpoint when the active setpoint changes value
        Cpl::Dm::Mp::Bool&              setpointChanged;        //!< When true, indicates that the current 'active' setpoint has changed value.  Note: this flag is NOT set if/when the active setpoint changes between heating/cooling modes
        Storm::Dm::MpPiConstants&       piConstants;            //!< PI constants (e.g. gain/reset terms) for the PI component
    };


protected:
    /// The Component's inputs
    Input_T  m_in;          

    /// The Component's outputs
    Output_T m_out;          

    /// Cached active setpoint value;
    float    m_prevActiveSetpoint;


public:
    /// Constructor.  
    PiPreProcess( struct Input_T ins, struct Output_T outs );

    /// Component specific initialization
    bool start( Cpl::System::ElapsedTime::Precision_T newInterval );

protected:
    /// See Storm::Component::Base
    bool execute( Cpl::System::ElapsedTime::Precision_T currentTick,
                  Cpl::System::ElapsedTime::Precision_T currentInterval );



protected:
    /** This method returns the configuration and run time input paratmers for
        Component. This method will be called from the context of the execute()
        method, i.e. every dt interval. The method returns true if successful;
        else false is return if an error was encounter in retrieving one or more
        the input parameters. When false is returned the 'errorOccurred' parameter
        of the execute() method is set to true and no additional Component
        processing is done for the current cycle.
    */
    virtual bool getInputs( Input_T& runtime ) = 0;

    /** This method is responsible for publishing/routing/pushing the
        Component's output upon completion of the current processing cycle.
        This method will be called from the context of the execute() method, i.e.
        every dt interval. The method returns true if successful; else false is
        return if an error was encounter in publishing/pushing one or more
        the output parameters. When false is returned the 'errorOccurred'
        parameter of the execute() method is set to true.
    */
    virtual bool setOutputs( Output_T& runtime ) = 0;



protected:
    /** Helper method that determines the maximum PI OUT value (aka load value)
        for cooling operation.  The default implementation supports N stages of
        compressor cooling
     */
    virtual float calcPiCoolingMaxOut( unsigned numCoolingStages );


    /** Helper method that determines the maximum PI OUT value (aka load value)
        for heating operation.  The default implementation only supports
        INDOOR heat (i.e. furnace + AC, or Electric Heat + AC)
     */
    virtual float calcPiHeatingMaxOut( unsigned numIndoorHeatingStage, Storm::Type::IduType iduType = Storm::Type::IduType::eFURNACE, unsigned numOutdoorHeatingStages=0, Storm::Type::OduType oduType = Storm::Type::OduType::eAC );


};



};      // end namespace
};      // end namespace
#endif  // end header latch
