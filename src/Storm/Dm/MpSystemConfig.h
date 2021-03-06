#ifndef Storm_Dm_MpSystemConfig_h_
#define Storm_Dm_MpSystemConfig_h_
/*-----------------------------------------------------------------------------
* This file is part of the Colony.Core Project.  The Colony.Core Project is an
* open source project with a BSD type of licensing agreement.  See the license
* agreement (license.txt) in the top/ directory or on the Internet at
* http://integerfox.com/colony.core/license.txt
*
* Copyright (c) 2014-2020  John T. Taylor
*
* Redistributions of the source code must retain the above copyright notice.
*----------------------------------------------------------------------------*/
/** @file */

#include "Cpl/Dm/ModelPointCommon_.h"
#include "Storm/Type/SystemConfig.h"
#include "Storm/Type/OperatingMode.h"
#include "Storm/Type/IduType.h"
#include "Storm/Type/OduType.h"


///
namespace Storm {
///
namespace Dm {


/** This class provides a concrete implementation for a Point who's data is the
    the System Configuration attributes.

    The toJSON() method is a read/modify operation, i.e. omitted key/value
    fields in the 'val' object are NOT updated.

    The toJSON()/fromJSON format is:
    \code

    { name:"<mpname>", type:"<mptypestring>", invalid:nn, seqnum:nnnn, locked:true|false, val:{ opMode:"<enum>", "oduType":"<enum>", "iduType":"<enum>", "numCompStages":n, "numIdStages":m, "totalStages":k, "fanCont":n, "gain":m.n, "reset":m.n, "maxPv":m.n, stages":[ {"stage":<num>, "lower":<val>, "upper":<val>, "minBlower":n, "maxBlower":m, "cph":"<enum>", "minOn":m, "minOff":n }...] }}

    \endcode


    NOTE: All methods in this class ARE thread Safe unless explicitly
          documented otherwise.
 */
class MpSystemConfig : public Cpl::Dm::ModelPointCommon_
{

protected:
    /// Storage for the MP's data
    Storm::Type::SystemConfig_T    m_data;

public:
    /** Constructor.  Valid MP.  Defaults all of the configuration to 'off configuration'
     */
    MpSystemConfig( Cpl::Dm::ModelDatabase& myModelBase, Cpl::Dm::StaticInfo& staticInfo);

public:
    /** Type safe read of the System Configuration
     */
    virtual int8_t read( Storm::Type::SystemConfig_T& configuration, uint16_t* seqNumPtr=0 ) const noexcept;

    /** Updates the entire System Configuration
     */
    virtual uint16_t write( Storm::Type::SystemConfig_T& newConfiguration, LockRequest_T lockRequest = eNO_REQUEST ) noexcept;

    /** Sets the model to its "Off" and/or default configuration settings
     */
    virtual uint16_t setToOff( LockRequest_T lockRequest = eNO_REQUEST ) noexcept;

    /** Public helper method to set a instance of the SystemConfig structure
        to its "Off" state
     */
    static void setToOff( Storm::Type::SystemConfig_T& cfgToReset ) noexcept;

    /// Type safe read-modify-write client callback interface
    typedef Cpl::Dm::ModelPointRmwCallback<Storm::Type::SystemConfig_T> Client;

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
    typedef Cpl::Dm::Subscriber<MpSystemConfig> Observer;

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
