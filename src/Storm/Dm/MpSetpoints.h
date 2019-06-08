#ifndef Storm_Dm_MpSetpoints_h_
#define Storm_Dm_MpSetpoints_h_
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

#include "Cpl/Dm/ModelPointCommon_.h"


/** This symbol defines the minimum delta (in degrees 'F) between the
    cooling and heating set-points (i.e. the cooling set-point is always
    greater than the heating set-point
 */
#ifndef OPTION_STORM_DM_MP_SETPOINTS_MIN_DELTA
#define OPTION_STORM_DM_MP_SETPOINTS_MIN_DELTA      4
#eendif


 ///
namespace Storm {
///
namespace Dm {


/** This class provides a concrete implementation for a Point who's data is the
    the house/room/zone cooling and heating set-points.  The cooling set-point MUST
    always be greater than the heating set-point.  All write operation ensure
    that the minimum delta (OPTION_STORM_DM_MP_SETPOINTS_MIN_DELTA) is enforced. 
    
    The toJSON() method is a read/modify operation, i.e. omitted key/value 
    fields in the 'val' object are NOT updated.

    The toJSON()/fromJSON format is:
    \code

    { name="<mpname>", type="<mptypestring>", invalid=nn, seqnum=nnnn, locked=true|false, val:{"cool"=mm.m, "heat"=nn.n} }

    \endcode


    NOTE: All methods in this class ARE thread Safe unless explicitly
          documented otherwise.
 */
class MpSetpoints : public Cpl::Dm::ModelPointCommon_
{
public:
    /** The MP's Data container.
     */
    typedef struct
    {
        float coolSetpt;        //!< Cooling set-point in degrees Fahrenheit.  The Cooling set-point must be >= heatSetpt+OPTION_STORM_DM_MP_SETPOINTS_MIN_DELTA
        float heatSetpt;        //!< Heating set-point in degrees Fahrenheit
    } Data;

protected:
    /// Storage for the MP's data
    Data                m_data;

public:
    /// Constructor.  Invalid MP
    MpSetpoints( Cpl::Dm::ModelDatabase& myModelBase, Cpl::Dm::StaticInfo& staticInfo );

    /// Constructor.  Valid MP
    MpSetpoints( Cpl::Dm::ModelDatabase& myModelBase, Cpl::Dm::StaticInfo& staticInfo, float coolSetpt, float heatSetpt );

public:
    /// See Cpl::Dm::ModelPoint
    uint16_t setInvalidState( int8_t newInvalidState, LockRequest_T lockRequest = eNO_REQUEST ) noexcept;


public:
    /** Type safe read of the Cooling set-point
     */
    virtual int8_t getCool( float& currentCoolSetpoint, uint16_t* seqNumPtr=0 ) const noexcept;

    /** Type safe read of the Heating set-point
     */
    virtual int8_t getHeat( float& currentHeatSetpoint, uint16_t* seqNumPtr=0 ) const noexcept;

    /** Sets the both the cooling and heating set-point.  If the specified
        set-points violates the minimum delta requirement, then the heating
        set-point is adjusted
     */
    virtual uint16_t write( float newCoolingSetpoint, float newHeatingSetpoint, LockRequest_T lockRequest = eNO_REQUEST ) noexcept;

    /** Sets the cooling set-point.  If the specified cooling set-point violates
        the minimum delta requirement, then the heating set-point is adjusted
     */
    virtual uint16_t setCool( float newSetpoint, LockRequest_T lockRequest = eNO_REQUEST ) noexcept;

    /** Sets the heating set-point.  If the specified cooling set-point violates
        the minimum delta requirement, then the cooling set-point is adjusted
     */
    virtual uint16_t setHeat( float newSetpoint, LockRequest_T lockRequest = eNO_REQUEST ) noexcept;

    /// Type safe read. See Cpl::Dm::ModelPoint
    virtual int8_t read( Data& dstData, uint16_t* seqNumPtr=0 ) const noexcept;

    /// Type safe write. See Cpl::Dm::ModelPoint
    virtual uint16_t write( const Data& srcData, LockRequest_T lockRequest = eNO_REQUEST ) noexcept;


    /// Type safe read-modify-write client callback interface
    typedef Cpl::Dm::ModelPointRmwCallback<Data> Client;

    /** Type safe read-modify-write. See Cpl::Dm::ModelPoint

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
    typedef Cpl::Dm::Subscriber<MpSetpoints> Observer;

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
