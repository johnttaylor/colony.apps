#ifndef Storm_Utils_SimHouse_h_
#define Storm_Utils_SimHouse_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2019  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

#include "Storm/Utils/SimSystem.h"

///
namespace Storm {
///
namespace Utils {


/** This class provides a 'functional' simulation of house with respect to
    Outdoor temperature, Indoor temperature, and active HVAC capacity.  Essentially,
    this class provide functional feedback for a thermostat's control algorithm.
    The feedback is NOT representative of actual house/thermal dynamics, but is
    sufficient to functional test a control algorithm.  This class uses the
    SimSystem class as basis for the feedback.

    Some terminology, notes, design, etc.

    maxOdt                  = Maximum Outdoor temperature for the simulation in degrees Fahrenheit
    minOdt                  = Minimum Outdoor temperature for the simulation in degrees Fahrenheit
    SimSystem.maxPotenial   = maxOdt - minOdt;
    odtCoolingLoadRating    = The outdoor temperature at which the HVAC cooling capacity is sized to maintain a 75' indoor temperature
    odtHeatingLoadRating    = The outdoor temperature at which the HVAC heating capacity is sized to maintain a 68' indoor temperature
    coolingCapacity         = 75' - odtCoolingLoadRating, e.g. 75' - 97'  = -22'
    heatingCapacity         = 68' - odtHeatingLoadRating, e.g. 68' - -10' = 78'
    percentActivityCapacity = Current active HVAC capacity. 0.0 = No active capacity, 1.0 = Full Capacity
    inPotOdt                = current outdoor temperature - minOdt
    InPotHVAC               = (coolingCapacity? coolingCapacity : heatingCapacity) * percentActivityCapacity
    SimSystem.InPot         = inPotOdt + InPotHVAC
    Indoor Temperature      = SimSystem.OutPot + minOdt
*/
class SimHouse
{
protected:
    /// System simulation
    SimSystem   m_sim;

    /// Maximum Outdoor temperature
    double      m_maxOdt;

    /// Minimum Outdoor temperature
    double      m_minOdt;

    /// Cooling capacity
    double      m_coolCap;

    /// Heating capacity
    double      m_heatCap;

    /// Environment resistance
    double      m_envResistance;

    /// Cooling resistance
    double      m_coolingEnvResistance;

    /// Heating resistance
    double      m_heatingEnvResistance;

public:
    /** Constructor.  See class description for argument semantics.

        Note: The systemXxxEnvResistance arguments determines the thermal transfer
              characteristics of the simulated house/system.  The larger the
              value, the slower the thermal transfer. See the SimSystem class
              for additional details for these arguments.
     */
    SimHouse( double tickPeriodInSeconds         = 1.0,    // i.e. how frequent the tick() method is called
              double initialOdt                  = 70.0,   // Initial outdoor temperature
              double maxOdt                      = 120.0,
              double minOdt                      = -20.0,
              double odtCoolingLoadRating        = 97.0,
              double odtHeatingLoadRating        = -10.0,
              double systemEnvResistance         = 10.0,    // Resistance used when NO active conditioning
              double systemHeatingEnvResistance  = 10.0,    // Resistance used when actively cooling
              double systemCoolingEnvResistance  = 35.0 );  // Resistance used when actively heating

public:
    /** This method is called every N seconds.  The call frequency MUST 
        match the tickPeriodInSeconds argument specified in the instance's
        constructor.  
        
        The method returns the newly calculated indoor temperature in degrees 
        Fahrenheit. 
     */
    double tick( double currentOdt, double percentActiveCapacity, bool coolingCapacity ) noexcept;
};



};      // end namespaces
};
#endif  // end header latch
