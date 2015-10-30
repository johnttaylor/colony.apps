#ifndef Storm_Thermostat_DataDictionary_h_
#define Storm_Thermostat_DataDictionary_h_
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


/// Namespaces
namespace Storm { namespace Thermostat {


/** This structure defines the data that is 'OWNED' (with respect to the
    RTE Model) by the Storm::Thermostat Application.  By classifying this data 
    as 'owned' by the Thermostat Applicaiton (and following that convention) - 
    the application can optimize its main loop/cycle processing in that it does 
    NOT have the Query the RTE Model for the this data since by defintion OWNED 
    data can ONLY be updated by ONE source - and I am that source!
 */
struct DataDictionary_T
{
public: 
    float               m_loadValue;                //!< The current Load Value 
    float               m_lvSumError;               //!< Sum Error term associated with m_loadValue 
    float               m_lvDeltaError;             //!< Delta Setpoint error used by the PI calculation
    int32_t             m_lvFreezeRefCount;         //!< Reference counter to freeze the output of the PI calculation
    int32_t             m_lvInhibitRefCount;        //!< Reference counter to inhibit the PI's integral term
    Storm::Type::Pulse  m_lvReset;                  //!< Pulse flag to request a reset of the PI block
    bool                m_lvInhibitedState          //!< Flag is true if PI's integral term is inhibited

public:
    Storm::Type::OMode::Enum_t  m_opMode;           //!< Actual/Operating mode for the thermostat
    Storm::Type::Pulse          m_opModeChanged;    //!< Indicates that there is/was an operating mode transition

public:                                                       
    Cpl::System::ElaspedTime::Precision_T   m_beginOnTime;      //!< The elasped time marker of when the system turned on any active Cooling/Heating
    Cpl::System::ElaspedTime::Precision_T   m_beginOffTime;     //!< The elasped time marker of when the system turned off all active Cooling/Heating
    bool                                    m_systemOn;         //!< Indicates that system is actively Cooling or Heating

};



};      // end namespace
};      // end namespace
#endif  // end header latch
