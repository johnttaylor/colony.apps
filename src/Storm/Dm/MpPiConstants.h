#ifndef Storm_Dm_MpPiConstants_h_
#define Storm_Dm_MpPiConstants_h_
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

#include "colony_config.h"
#include "Cpl/Dm/ModelPointCommon_.h"


/** This symbol defines the nominal PI Gain constant for cooling
 */
#ifndef OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_NORMAL_GAIN
#define OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_NORMAL_GAIN     15.0F
#endif

/** This symbol defines the nominal PI reset constant for cooling.  The
    time units are milliseconds.
 */
#ifndef OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_NORMAL_RESET
#define OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_NORMAL_RESET    ((5*60*1000) / OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_NORMAL_GAIN)   // 5 min reset time
#endif

/** This symbol defines the nominal PI Gain constant for heating
 */
#ifndef OPTION_STORM_DM_MP_PI_CONSTANTS_HEATING_NORMAL_GAIN
#define OPTION_STORM_DM_MP_PI_CONSTANTS_HEATING_NORMAL_GAIN     20.0F
#endif

/** This symbol defines the nominal PI reset constant for heating. The
    time units are milliseconds.
 */
#ifndef OPTION_STORM_DM_MP_PI_CONSTANTS_HEATING_NORMAL_RESET
#define OPTION_STORM_DM_MP_PI_CONSTANTS_HEATING_NORMAL_RESET    ((5*60*1000) / OPTION_STORM_DM_MP_PI_CONSTANTS_HEATING_NORMAL_GAIN)   // 5 min reset time
#endif

/** This symbol defines the 'fast' PI Gain constant for cooling
 */
#ifndef OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_FAST_GAIN
#define OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_FAST_GAIN       20.0F
#endif

/** This symbol defines the 'fast' PI reset constant for cooling. The
    time units are milliseconds.
 */
#ifndef OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_FAST_RESET
#define OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_FAST_RESET      ((5*60*1000) / OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_FAST_GAIN)   // 5 min reset time
#endif

/** This symbol defines the 'fast' PI Gain constant for heating
 */
#ifndef OPTION_STORM_DM_MP_PI_CONSTANTS_HEATING_FAST_GAIN
#define OPTION_STORM_DM_MP_PI_CONSTANTS_HEATING_FAST_GAIN       30.0F
#endif

/** This symbol defines the 'fast' PI reset constant for heating. The
    time units are milliseconds.
 */
#ifndef OPTION_STORM_DM_MP_PI_CONSTANTS_HEATING_FAST_RESET
#define OPTION_STORM_DM_MP_PI_CONSTANTS_HEATING_FAST_RESET      ((5*60*1000) / OPTION_STORM_DM_MP_PI_CONSTANTS_HEATING_FAST_GAIN)   // 5 min reset time
#endif

/** This symbol defines the default maximum Process Variable's output
    value.
 */
#ifndef OPTION_STORM_DM_MP_PI_CONSTANTS_MAX_PV_OUT
#define OPTION_STORM_DM_MP_PI_CONSTANTS_MAX_PV_OUT              100
#endif

        ///
namespace Storm {
///
namespace Dm {


/** This class provides a concrete implementation for a Point who's data is a
    set of PI constants (i.e. gain and reset parameters)

    The toJSON() method is a modify operation, i.e. both the gain and reset
    fields MUST always been updated/specified when changing data values.

    The toJSON()/fromJSON format is:
    \code

    { name:"<mpname>", type:"<mptypestring>", invalid:nn, seqnum:nnnn, locked:true|false, val:{"gain":mm.m, "reset":nn.n} }

    \endcode


    NOTE: All methods in this class ARE thread Safe unless explicitly
          documented otherwise.
 */
class MpPiConstants : public Cpl::Dm::ModelPointCommon_
{
public:
    /** The MP's Data container.
     */
    typedef struct
    {
        float gain;         //!< The proportional gain constant for the PI
        float reset;        //!< The PI Reset time in milliseconds. The reset time is the time it takes for the integral term, given a constant error, to effect PI OUT term the same amount as the Proportional gain.
        float maxPvOut;     //!< The maximum allowed 'Process Value' value output of the PI
    } Data;

protected:
    /// Storage for the MP's data
    Data                m_data;

public:
    /// Constructor.  Valid MP
    MpPiConstants( Cpl::Dm::ModelDatabase& myModelBase, Cpl::Dm::StaticInfo& staticInfo, float gain=OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_NORMAL_GAIN, float reset=OPTION_STORM_DM_MP_PI_CONSTANTS_COOLING_NORMAL_RESET, float maxPvOut = OPTION_STORM_DM_MP_PI_CONSTANTS_MAX_PV_OUT );

public:
    /** Type safe read of the Cooling & Heating set-point
     */
    virtual int8_t read( float& gain, float& reset, float& maxPvOut, uint16_t* seqNumPtr=0 ) const noexcept;

    /** Sets the both the cooling and heating set-point.  If the specified
        set-points violates the minimum delta requirement, then the heating
        set-point is adjusted
     */
    virtual uint16_t write( float gain, float reset, float maxPvOut, LockRequest_T lockRequest = eNO_REQUEST ) noexcept;

    /// Type safe read-modify-write client callback interface
    typedef Cpl::Dm::ModelPointRmwCallback<Data> Client;

    /** Type safe read-modify-write. See Cpl::Dm::ModelPoint.

        NOTE: The client is responsible for enforcing the min/max set-point
              ranges and minimum delta requirements for the set-point values
              (see the validateSetpoints() method).

        NOTE: THE USE OF THIS METHOD IS STRONGLY DISCOURAGED because it has
              potential to lockout access to the ENTIRE Model Base for an
              indeterminate amount of time.  And alternative is to have the
              concrete Model Point leaf classes provide the application
              specific read, write, read-modify-write methods in addition or in
              lieu of the read/write methods in this interface.
     */
    virtual uint16_t readModifyWrite( Client& callbackClient, LockRequest_T lockRequest = eNO_REQUEST );


public:
    /// Type safe subscriber
    typedef Cpl::Dm::Subscriber<MpPiConstants> Observer;

    /// Type safe register observer
    virtual void attach( Observer& observer, uint16_t initialSeqNumber=SEQUENCE_NUMBER_UNKNOWN ) noexcept;

    /// Type safe un-register observer
    virtual void detach( Observer& observer ) noexcept;


public:
    /// See Cpl::Dm::Point.  
    bool toJSON( char* dst, size_t dstSize, bool& truncated, bool verbose=true ) noexcept;

    ///  See Cpl::Dm::ModelPoint.
    const char* getTypeAsText() const noexcept;

    /// See Cpl::Dm::ModelPoint.  Note: the returned sized does DOES NOT the null terminator
    size_t getSize() const noexcept;


public:
    /// See Cpl::Dm::Point.  
    bool fromJSON_( JsonVariant& src, LockRequest_T lockRequest, uint16_t& retSequenceNumber, Cpl::Text::String* errorMsg ) noexcept;

    /// See Cpl::Dm::ModelPoint. 
    void copyDataTo_( void* dstData, size_t dstSize ) const noexcept;

    /// See Cpl::Dm::ModelPoint.  
    void copyDataFrom_( const void* srcData, size_t srcSize ) noexcept;

    /// See Cpl::Dm::ModelPoint.  
    bool isDataEqual_( const void* otherData ) const noexcept;

    /// See Cpl::Dm::ModelPoint.  
    const void* getImportExportDataPointer_() const noexcept;

    /// See Cpl::Dm::ModelPoint.  
    size_t getInternalDataSize_() const noexcept;
};



};      // end namespaces
};
#endif  // end header latch
