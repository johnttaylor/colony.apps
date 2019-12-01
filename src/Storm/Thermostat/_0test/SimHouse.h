#ifndef Storm_Thermostat_0test_SimHouse_h_
#define Storm_Thermostat_0test_SimHouse_h_
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

#include "Storm/Utils/SimHouse.h"
#include "Cpl/System/EventLoop.h"


/** This class glues the House Simulation into the Thermostat Test Application.
    Updates the mp_primaryRawIdt ModelPoint with the output of simulation - when
    the mp_houseSimEnabled MP is true.  The current outdoor temperature use
    by the simulation is set/access by the mp_outdoorTemp MP.
 */
class SimHouse : public Cpl::System::EventLoop
{
public:
    /// Constructor
    SimHouse();

protected:
    /// Override base implementation to access the event-loop timing directly
    void appRun();

    /// Helper method
    void executeSimulation();
protected:
    Storm::Utils::SimHouse  m_sim;
};

#endif  // end header latch
