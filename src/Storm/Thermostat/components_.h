#ifndef Storm_Thermostat_components_h_
#define Storm_Thermostat_components_h_
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
/** @file */

#include "Storm/Thermostat/points_.h"
#include "Storm/Rte/Point/Operate.h"
#include "Storm/Rte/Point/Sensors.h"
#include "Storm/Rte/Point/UserConfig.h"
#include "Storm/Rte/Point/InstallerConfig.h"


/// Namespaces
namespace Storm { namespace Thermostat {


/////////////////////////////////////////////////////////////////////
// OPERATING MODE
/////////////////////////////////////////////////////////////////////

/// This concrete implements the "IO Layer" of its Parent's class
class OperatingMode: public Storm::Component::OperatingMode
{
protected:
    DataDictionary&                     m_dd;           //!< Application Owned data
    Storm::Rte::Point::Operate&         m_cfg;          //!< Model Data
    Storm::Rte::Point::InstallerConfig& m_installerCfg; //!< Model Data
    Storm::Rte::Point::Sensors&         m_sensors;      //!< Model Data

public:
    /// Constructor
    OperatingMode( DataDictionary& myDdd, Storm::Rte::Point::Operate& operateConfig, Storm::Rte::Point::InstallerConfig& installerCfg, Storm::Rte::Point& sensors )
        :m_dd(myDdd),m_cfg(operateConfig),m_installerCfg(installerCfg),m_sensors(sensors)
            {}

public:

protected:
    /// See Storm::Component::OperatingMode
    bool getInputs( Input_T& runtime )
        {
        runtime.m_freezePiRefCount  = m_dd.m_lv.m_freezeRefCount.get();
        runtime.m_resetPi.m_flag    = m_dd.m_lv.m_reset.get();         
        runtime.m_beginOffTime      = m_dd.m_sysState.m_beginOffTime.get();    
        runtime.m_systemOn          = m_dd.m_sysState.m_systemOn.get();        
        runtime.m_coolingSetpoint   = m_cfg.m_operate.m_coolSetpoint.get();
        runtime.m_heatingSetpoint   = m_cfg.m_operate.m_heatSetpoint.get(); 
        runtime.m_userMode          = m_cfg.m_operate.m_mode.get();
        runtime.m_idt               = getIdt( runtime.m_idtIsValid );
        }

    /// See Storm::Component::OperatingMode
    bool setOutputs( Output_T& runtime )
        {
        m_dd.m_lv.m_freezePiRefCount.set( runtime.m_freezePiRefCount );
        m_dd.m_lv.m_reset.set( runtime.m_resetPi.m_flag );         
        m_dd.m_operate.m_opMode.set( runtime.m_opMode );
        m_dd.m_operate.m_opModeChanged.set( runtime.m_opModeChanged.m_flag );
        }


protected:
    /// Helper method
    float getIdt( bool& isValid )
        {
        if ( m_installerCfg.m_equipConfig.m_haveRemoteIdtSensor.get() && m_sensors.m_sensors.m_ridt.isValid() )
            {
            return m_sensors.m_sensors.m_ridt.get();
            isValid = true;
            }

        isValid = m_sensors.m_sensors.m_idt.isValid();
        return m_sensors.m_sensors.m_idt.get()
        }

};



/////////////////////////////////////////////////////////////////////
// PI CONTEXT IDT
/////////////////////////////////////////////////////////////////////

/// This concrete implements the "IO Layer" of its Parent's class
class PiContextIdt: public Storm::Component::PiContextIdt
{
protected:
    DataDictionary&                      m_dd;          //!< Application Owned data
    Storm::Rte::Point::Operate&          m_opCfg;       //!< Model Data
    Storm::Rte::Point::UserConfig&       m_userCfg;     //!< Model Data
    Storm::Rte::Point::InstallerConfig&  m_installerCfg;//!< Model Data
    Storm::Rte::Point::Sensors&          m_sensors;     //!< Model Data

public:
    /// Constructor
    PiContextIdt( DataDictionary& myDD, Storm::Rte::Point::Operate& operateConfig, Storm::Rte::Point;:UserConfig& userConfig, Storm::Rte::Point::InstallerConfig& installerCfg, Storm::Rte::Point& sensors )
        :m_dd(myDD),m_opCfg(operateConfig),m_userCfg(userConfig),m_installerCfg(installerCfg),m_sensors(sensors)
            {}

protected:
    /// See Storm::Component::PiContextIdt
    bool getInputs( Input_T& runtime )
        {
        runtime.m_coolingGain0          = m_installerCfg.m_piConfig.m_coolingGain0.get();                                 
        runtime.m_coolingGain1          = m_installerCfg.m_piConfig.m_coolingGain1.get();                                 
        runtime.m_coolingResetTime0     = m_installerCfg.m_piConfig.m_coolingResetTime0.get();                                 
        runtime.m_coolingResetTime1     = m_installerCfg.m_piConfig.m_coolingResetTime1.get();                                 
        runtime.m_heatingGain0          = m_installerCfg.m_piConfig.m_heatingGain0.get();                                      
        runtime.m_heatingGain1          = m_installerCfg.m_piConfig.m_heatingGain1.get();                                      
        runtime.m_heatingResetTime0     = m_installerCfg.m_piConfig.m_heatingResetTime0.get();                                 
        runtime.m_heatingResetTime1     = m_installerCfg.m_piConfig.m_heatingResetTime1.get();                                 
        runtime.m_coolingNumStages      = m_installerCfg.m_equipConfig.m_numCompCoolingStages.get();                                 
        runtime.m_primaryHeatSource     = m_installerCfg.m_equipConfig.m_primaryHeatType.get();                                 
        runtime.m_secondaryHeatSource   = m_installerCfg.m_equipConfig.m_backupHeatType.get();  
        runtime.m_heatingNumPriStages   = getNumHeatStages( runtime.m_primaryHeatSource );                               
        runtime.m_heatingNumSecStages   = getNumHeatStages( runtime.m_secondaryHeatSource );                               
        runtime.m_coolingSetpoint       = m_opCfg.m_operate.m_coolSetpoint.get();                                 
        runtime.m_heatingSetpoint       = m_opCfg.m_operate.m_heatSetpoint.get();                                 
        runtime.m_noPrimaryHeat         = m_opCfg.m_operate.m_noPrimaryHeat.get();        
        runtime.m_opMode                = m_dd.m_operate.m_opMode.get();                    
        runtime.m_opModeChanged.m_flag  = m_dd.m_operate.m_opModeChanged.get();             
        runtime.m_coolingFastPiEnabled  = m_userConfig.m_config.m_fastCoolingEnabled.get();
        runtime.m_heatingFastPiEnabled  = m_userConfig.m_config.m_fastHeatingEnabled.get();
        runtime.m_idt                   = getIdt()
        }

    /// See Storm::Component::PiContextIdt
    bool setOutputs( Output_T& runtime )
        {
        m_dd.m_lv.m_deltaError.set( runtime.m_deltaError );     
        m_dd.m_lv.m_gain.set( runtime.m_gain );           
        m_dd.m_lv.m_resetTime.set( runtime.m_resetTime );      
        m_dd.m_lv.m_maxValue.set( runtime.m_maxOutValue );    
        m_dd.m_operate.m_deltaSetpoint.set( runtime.m_deltaSetpoint );  
        m_dd.m_operate.m_setpointChanged.set( runtime.m_setpointChanged.m_flag );
        }


protected:
    /// Helper method
    unsigned getNumHeatStages( Storm::Type::HeatType source )
        {
        unsigned result = 0;

        switch( source )
            {
            case Storm::Type::HeatType::eFOSSIL:
                result = m_installerCfg.m_equipConfig.m_numFossilHeatingStages.get();
                break;

            case Storm::Type::HeatType::eELECTRIC:
                result = m_installerCfg.m_equipConfig.m_numElecHeatingStages.get();
                break;

            case Storm::Type::HeatType::eMECHANICAL:
                result = m_installerCfg.m_equipConfig.m_numCompHeatingStages.get();
                break;

            default:
                break;
            }

        return result;
        }

    /// Helper method
    float getIdt()
        {
        if ( m_installerCfg.m_equipConfig.m_haveRemoteIdtSensor.get() && m_sensors.m_sensors.m_ridt.isValid() )
            {
            return m_sensors.m_sensors.m_ridt.get();
            }

        return m_sensors.m_sensors.m_idt.get()
        }

};


/////////////////////////////////////////////////////////////////////
// PI 
/////////////////////////////////////////////////////////////////////

/// This concrete implements the "IO Layer" of its Parent's class
class Pi: public Storm::Component::Pi
{
protected:
    DataDictionary&  m_dd;       //!< Application Owned data

public:
    /// Constructor
    Pi( DataDictionary& myDD ):m_dd(myDD) {}

protected:
    /// See Storm::Component::Pi
    bool getInputs( Input_T& runtime )
        {
        runtime.m_deltaError        = m_dd.m_lv.m_deltaError.get();     
        runtime.m_freezeRefCount    = m_dd.m_lv.m_freezeRefCount.get(); 
        runtime.m_inhibitRefCount   = m_dd.m_lv.m_inhibitRefCount.get();
        runtime.m_gain              = m_dd.m_lv.m_gain.get();
        runtime.m_resetTime         = m_dd.m_lv.m_resetTime.get();     
        runtime.m_maxOutValue       = m_dd.m_lv.m_maxValue.get();
        runtime.m_reset.m_flag      = m_dd.m_lv.m_reset.get();      
        }


    /// See Storm::Component::Pi
    bool setOutputs( Output_T& runtime )
        {
        m_dd.m_lv.m_value.set( runtime.m_out );         
        m_dd.m_lv.m_sumError.set( runtime.m_sumError );         
        m_dd.m_lv.m_inhibitedState.set( runtime.m_inhibitedState );         
        }
};





};      // end namespace
};      // end namespace
#endif  // end header latch
