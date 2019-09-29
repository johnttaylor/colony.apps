#ifndef Storm_Dm_MpComfortConfig_h_
#define Storm_Dm_MpComfortConfig_h_
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
#include "Storm/Constants.h"
#include "Storm/Type/Cph.h"



///
namespace Storm {
///
namespace Dm {


/** This class provides a concrete implementation for a Point who's data is the
    the Comfort configuration attributes.

    The toJSON() method is a read/modify operation, i.e. omitted key/value
    fields in the 'val' object are NOT updated.

    The toJSON()/fromJSON format is:
    \code

    { name:"<mpname>", type:"<mptypestring>", invalid:nn, seqnum:nnnn, locked:true|false, val:{ "cool":[ {"stage":<num>, "cph":<enum>, "minOn":<secs> "minOff":<secs>}, ...]
                                                                                                "heat":[ {"stage":<num>, "cph":<enum>, "minOn":<secs> "minOff":<secs>}, ...] }}

    \endcode


    NOTE: All methods in this class ARE thread Safe unless explicitly
          documented otherwise.
 */
class MpComfortConfig : public Cpl::Dm::ModelPointCommon_
{
public:
    /** The 'comfort' parameters per stage 
     */
    typedef struct
    {
        int         cph;         //!< Cycle-Per-Hour settings.   The actual type is: Storm::Type::Cph ('betterenums' do not play well with classes/struct)
        uint32_t    minOnTime;   //!< Minimum on time
        uint32_t    minOffTime;  //!< Minimum off time
    } Parameters_T;

    /** The MP's Data container.
     */
    typedef struct
    {
        Parameters_T cooling[OPTION_STORM_MAX_COMPRESSOR_COOLING_STAGES]; //!< Parameters for each cooling stages.  Cooling stages are zero-index, i.e. cooling stage1 is index 0
        Parameters_T heating[OPTION_STORM_MAX_HEATING_STAGES]; //!< Parameters for each heating stages.  Heating stages are zero-index, i.e. cooling stage1 is index 0.  Compressor heating stages (if any) are ordered first (i.e. lower index) than the indoor heat stages.  For example a 2 stage HP with a 1 stage furnace: index 0 = 1st stage compressor, 1=2nd compressor stage, 2= 1st furnace stage
    } Data;

protected:
    /// Storage for the MP's data
    Data                m_data;

public:
    /** Constructor.  Valid MP.  Defaults all stages (for both modes) to 3CPH, 5min minimum on time, 5min minimum off time
     */
    MpComfortConfig( Cpl::Dm::ModelDatabase& myModelBase, Cpl::Dm::StaticInfo& staticInfo);

public:
    /** Method that limit/range checks the specified configuration. Invalid and/or
        out-of-range value are corrected, Returns true if one or more fields
        were corrected.
     */
    virtual bool validate( Data& src ) const noexcept;

    /** Method that limit/range checks the specified STAGE configuration. Invalid 
        and/or out-of-range value are corrected, Returns true if one or more 
        fields were corrected.
     */
    virtual bool validate( Parameters_T& src ) const noexcept;

public:
    /** Type safe read of the Outdoor Unit Configuration
     */
    virtual int8_t read( Data& configuration, uint16_t* seqNumPtr=0 ) const noexcept;

    /** Updates the entire Outdoor Unit Configuration
     */
    virtual uint16_t write( Data& newConfiguration, LockRequest_T lockRequest = eNO_REQUEST ) noexcept;

    /** Updates only the specified cooling stage (i.e. this is a read-modify-write operation)
     */
    virtual uint16_t writeCoolingStage( uint8_t stageIndex, Parameters_T newStageParameters, LockRequest_T lockRequest = eNO_REQUEST ) noexcept;

    /** Updates only the specified heating stage (i.e. this is a read-modify-write operation)
     */
    virtual uint16_t writeHeatingStage( uint8_t stageIndex, Parameters_T newStageParameters, LockRequest_T lockRequest = eNO_REQUEST ) noexcept;


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

    /// See Cpl::Dm::ModelPoint
    uint16_t setInvalidState( int8_t newInvalidState, LockRequest_T lockRequest = eNO_REQUEST ) noexcept;


public:
    /// Type safe subscriber
    typedef Cpl::Dm::Subscriber<MpComfortConfig> Observer;

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
