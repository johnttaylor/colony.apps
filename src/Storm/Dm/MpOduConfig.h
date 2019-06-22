#ifndef Storm_Dm_MpOduConfig_h_
#define Storm_Dm_MpOduConfig_h_
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
#include "Storm/Type/OduType.h"


/// Maximum number of allowed compressor stages
#ifndef OPTION_STORM_DM_ODU_CONFIG_MAX_COMPRESSOR_STAGES
#define OPTION_STORM_DM_ODU_CONFIG_MAX_COMPRESSOR_STAGES        1
#endif

/// Default Indoor Unit type
#ifndef OPTION_STORM_DM_ODU_CONFIG_DEFAULT_ODUTYPE
#define OPTION_STORM_DM_ODU_CONFIG_DEFAULT_ODUTYPE              Storm::Type::OduType::eAC
#endif

/// Default number of indoor heating stages
#ifndef OPTION_STORM_DM_ODU_CONFIG_DEFAULT_NUM_STAGES
#define OPTION_STORM_DM_ODU_CONFIG_DEFAULT_NUM_STAGES           1
#endif

///
namespace Storm {
///
namespace Dm {


/** This class provides a concrete implementation for a Point who's data is the
    the Outdoor Unit configuration attributes.

    The toJSON() method is a read/modify operation, i.e. omitted key/value
    fields in the 'val' object are NOT updated.

    The toJSON()/fromJSON format is:
    \code

    { name:"<mpname>", type:"<mptypestring>", invalid:nn, seqnum:nnnn, locked:true|false, val:{"type":"<enum>", "numStages":n} }

    \endcode


    NOTE: All methods in this class ARE thread Safe unless explicitly
          documented otherwise.
 */
class MpOduConfig : public Cpl::Dm::ModelPointCommon_
{
public:
    /** The MP's Data container.
     */
    typedef struct
    {
        int      type;        //!< Type of Indoor Unit.  Actual type is Storm::Type::OduType  (Note: BETTER_ENUMs and classes/structs don't mix well - so we have to use an int in the struct)
        uint16_t numStages;   //!< Number of Compressor stages in the Outdoor Unit 
    } Data;

protected:
    /// Storage for the MP's data
    Data                m_data;

public:
    /// Constructor.  Valid MP
    MpOduConfig( Cpl::Dm::ModelDatabase& myModelBase, Cpl::Dm::StaticInfo& staticInfo, Storm::Type::OduType unitType=OPTION_STORM_DM_ODU_CONFIG_DEFAULT_ODUTYPE, uint16_t numCompressorStages=OPTION_STORM_DM_ODU_CONFIG_DEFAULT_NUM_STAGES );

public:
    /** Method that limit/range checks the specified configuration. Invalid and/or
        out-of-range value are corrected, Returns true if one or more fields
        were corrected.
     */
    virtual bool validate( Data& src ) const noexcept;

public:
    /** Type safe read of the Outdoor Unit Configuration
     */
    virtual int8_t read( Data& configuration, uint16_t* seqNumPtr=0 ) const noexcept;

    /** Updates the entire Outdoor Unit Configuration
     */
    virtual uint16_t write( Data& newConfiguration, LockRequest_T lockRequest = eNO_REQUEST ) noexcept;

    /** Updates the unit type
     */
    virtual uint16_t writeType( Storm::Type::OduType newUnitType, LockRequest_T lockRequest = eNO_REQUEST ) noexcept;

    /** Updates the number of indoor heating stages. Note: if 'numStages'
        is greater than the supported maximum, it is clamp at the max supported
        value.
     */
    virtual uint16_t writeCompressorStages( uint16_t numStages, LockRequest_T lockRequest = eNO_REQUEST ) noexcept;


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
    typedef Cpl::Dm::Subscriber<MpOduConfig> Observer;

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