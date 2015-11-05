#ifndef Storm_Rte_Tuple_EquipConfig_h_
#define Storm_Rte_Tuple_EquipConfig_h_
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

#include "Rte/Tuple/Basic.h"
#include "Rte/Element/Basic.h"
#include "Storm/Rte/TMode.h"




#define STORM_RTE_TUPLE_EQUIPCONFIG_NUM_COMP_COOLING_STAGES     0   //!< Element Index
#define STORM_RTE_TUPLE_EQUIPCONFIG_NUM_COMP_HEATING_STAGES     1   //!< Element Index
#define STORM_RTE_TUPLE_EQUIPCONFIG_NUM_ELEC_HEATING_STAGES     2   //!< Element Index
#define STORM_RTE_TUPLE_EQUIPCONFIG_NUM_FOSSIL_EATING_STAGES    3   //!< Element Index
#define STORM_RTE_TUPLE_EQUIPCONFIG_PRIMARY_HEAT_TYPE           4   //!< Element Index
#define STORM_RTE_TUPLE_EQUIPCONFIG_BACKUP_HEAT_TYPE            5   //!< Element Index
#define STORM_RTE_TUPLE_EQUIPCONFIG_HAVE_VSPD_BLOWER            6   //!< Element Index
#define STORM_RTE_TUPLE_EQUIPCONFIG_HAVE_REMOTE_IDT_SENSOR      7   //!< Element Index
#define STORM_RTE_TUPLE_EQUIPCONFIG_HAVE_IDRH_SENSOR            8   //!< Element Index
#define STORM_RTE_TUPLE_EQUIPCONFIG_HAVE_ODT_SENSOR             9   //!< Element Index
#define STORM_RTE_TUPLE_EQUIPCONFIG_HAVE_ODRH_SENSOR            10  //!< Element Index
#define STORM_RTE_TUPLE_EQUIPCONFIG_HAVE_HUMIDSTAT              11  //!< Element Index
#define STORM_RTE_TUPLE_OPERATE_NUM_TUPLES                      (STORM_RTE_TUPLE_EQUIPCONFIG_HAVE_HUMIDSTAT+1) //!< Number of Elements



/// Namespaces
namespace Storm { namespace Rte { namespace Tuple {


/** Tuple for the User operating mode, setpoints, fan mode, etc. a thermostat
 */
class EquipConfig: public Rte::Tuple::Basic<STORM_RTE_TUPLE_OPERATE_NUM_TUPLES>
{
public: 
    /// Number of compressor cooling stages (0-N)
    Rte::Element::Uinteger8_T   m_numCompCoolingStages;

    /// Number of compressor heating stages (0-N)
    Rte::Element::Uinteger8_T   m_numCompHeatingStages;

    /// Number of Electric heating stages (0-N)
    Rte::Element::Uinteger8_T   m_numElecHeatingStages;

    /// Number of Furnace heating stages (0-N)
    Rte::Element::Uinteger8_T   m_numFossilHeatingStages;

    /// Type of the primary heating source
    Storm::Rte::HeatType        m_primaryHeatType;

    /// Type of the secondary heating source
    Storm::Rte::HeatType        m_backupHeatType;

    /// Indicates that the indoor blower motor is a variable speed motor
    Rte::Element::Boolean_T     m_haveVspdBlower;

    /// Indicates that Remote IDT sensor is available
    Rte::Element::Boolean_T     m_haveRemoteIdtSensor;

    /// Indicates that Indoor humidity is available
    Rte::Element::Boolean_T     m_haveIdrhSensor;

    /// Indicates that Outdoor temperature is available
    Rte::Element::Boolean_T     m_haveOdtSensor;

    /// Indicates that Outdoor humidity is available
    Rte::Element::Boolean_T     m_haveOdrhSensor;

    /// Indicates that Humistat has been connected to the system
    Rte::Element::Boolean_T     m_haveHumidstat;


public:
    /// Constructor
    EquipConfig( void )
        {
        registerElement( STORM_RTE_TUPLE_EQUIPCONFIG_NUM_COMP_COOLING_STAGES,  m_numCompCoolingStages   );
        registerElement( STORM_RTE_TUPLE_EQUIPCONFIG_NUM_COMP_HEATING_STAGES,  m_numCompHeatingStages   );
        registerElement( STORM_RTE_TUPLE_EQUIPCONFIG_NUM_ELEC_HEATING_STAGES,  m_numElecHeatingStages   );
        registerElement( STORM_RTE_TUPLE_EQUIPCONFIG_NUM_FOSSIL_EATING_STAGES, m_numFossilHeatingStages );
        registerElement( STORM_RTE_TUPLE_EQUIPCONFIG_PRIMARY_HEAT_TYPE,        m_primaryHeatType        );
        registerElement( STORM_RTE_TUPLE_EQUIPCONFIG_BACKUP_HEAT_TYPE,         m_backupHeatType         );
        registerElement( STORM_RTE_TUPLE_EQUIPCONFIG_HAVE_VSPD_BLOWER,         m_haveVspdBlower         );
        registerElement( STORM_RTE_TUPLE_EQUIPCONFIG_HAVE_REMOTE_IDT_SENSOR,   m_haveRemoteIdtSensor    );
        registerElement( STORM_RTE_TUPLE_EQUIPCONFIG_HAVE_IDRH_SENSOR,         m_haveIdrhSensor         );
        registerElement( STORM_RTE_TUPLE_EQUIPCONFIG_HAVE_ODT_SENSOR,          m_haveOdtSensor          );
        registerElement( STORM_RTE_TUPLE_EQUIPCONFIG_HAVE_ODRH_SENSOR,         m_haveOdrhSensor         );
        registerElement( STORM_RTE_TUPLE_EQUIPCONFIG_HAVE_HUMIDSTAT,           m_haveHumidstat          );
        }                                                             

};


};      // end namespace
};     
};     
#endif  // end header latch
