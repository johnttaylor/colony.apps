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


#include "MpPiConstants.h"
#include "Cpl/System/Assert.h"
#include "Cpl/System/FatalError.h"
#include "Cpl/Math/real.h"
#include <string.h>

///
using namespace Storm::Dm;


///////////////////////////////////////////////////////////////////////////////
MpPiConstants::MpPiConstants( Cpl::Dm::ModelDatabase& myModelBase, Cpl::Dm::StaticInfo& staticInfo, float gain, float reset, float maxPvOut )
    : ModelPointCommon_( myModelBase, &m_data, staticInfo, MODEL_POINT_STATE_VALID )
    , m_data( { gain, reset, maxPvOut } )
{
}

///////////////////////////////////////////////////////////////////////////////
int8_t MpPiConstants::read( float& gain, float& reset, float& maxPvOut, uint16_t * seqNumPtr ) const noexcept
{
    Data   dst;
    int8_t valid = ModelPointCommon_::read( &dst, sizeof( Data ), seqNumPtr );
    gain         = dst.gain;
    reset        = dst.reset;
    maxPvOut     = dst.maxPvOut;
    return valid;
}

uint16_t MpPiConstants::write( float gain, float reset, float maxPvOut, LockRequest_T lockRequest ) noexcept
{
    Data src = { gain, reset, maxPvOut };
    return ModelPointCommon_::write( &src, sizeof( Data ), lockRequest );
}

uint16_t MpPiConstants::readModifyWrite( Client & callbackClient, LockRequest_T lockRequest )
{
    return ModelPointCommon_::readModifyWrite( callbackClient, lockRequest );
}

void MpPiConstants::attach( Observer & observer, uint16_t initialSeqNumber ) noexcept
{
    ModelPointCommon_::attach( observer, initialSeqNumber );
}

void MpPiConstants::detach( Observer & observer ) noexcept
{
    ModelPointCommon_::detach( observer );
}

bool MpPiConstants::isDataEqual_( const void* otherData ) const noexcept
{
    Data* otherDataPtr = ( Data*) otherData;

    return Cpl::Math::areFloatsEqual( m_data.gain, otherDataPtr->gain ) &&
        Cpl::Math::areFloatsEqual( m_data.reset, otherDataPtr->reset ) &&
        Cpl::Math::areFloatsEqual( m_data.maxPvOut, otherDataPtr->maxPvOut );

}

void MpPiConstants::copyDataTo_( void* dstData, size_t dstSize ) const noexcept
{
    CPL_SYSTEM_ASSERT( dstSize == sizeof( Data ) );
    Data* dstDataPtr   = ( Data*) dstData;
    *dstDataPtr        = m_data;
}

void MpPiConstants::copyDataFrom_( const void* srcData, size_t srcSize ) noexcept
{
    CPL_SYSTEM_ASSERT( srcSize == sizeof( Data ) );
    Data* dataSrcPtr   = ( Data*) srcData;
    m_data             = *dataSrcPtr;
}


///////////////////////////////////////////////////////////////////////////////
const char* MpPiConstants::getTypeAsText() const noexcept
{
    return "Storm::Dm::MpPiConstants";
}

size_t MpPiConstants::getSize() const noexcept
{
    return sizeof( Data );
}

size_t MpPiConstants::getInternalDataSize_() const noexcept
{
    return sizeof( Data );
}


const void* MpPiConstants::getImportExportDataPointer_() const noexcept
{
    return &m_data;
}

bool MpPiConstants::toJSON( char* dst, size_t dstSize, bool& truncated, bool verbose ) noexcept
{
    // Get my state
    m_modelDatabase.lock_();
    uint16_t seqnum = m_seqNum;
    int8_t   valid  = m_validState;
    bool     locked = m_locked;

    // Start the conversion
    JsonDocument& doc = beginJSON( valid, locked, seqnum, verbose );

    // Construct the 'val' key/value pair 
    if ( IS_VALID( valid ) )
    {
        JsonObject valObj  = doc.createNestedObject( "val" );
        valObj["gain"]     = m_data.gain;
        valObj["reset"]    = m_data.reset;
        valObj["maxPvOut"] = m_data.maxPvOut;
    }

    // End the conversion
    endJSON( dst, dstSize, truncated, verbose );

    m_modelDatabase.unlock_();
    return true;
}

bool MpPiConstants::fromJSON_( JsonVariant & src, LockRequest_T lockRequest, uint16_t & retSequenceNumber, Cpl::Text::String * errorMsg ) noexcept
{
    float gain         = m_data.gain;
    float reset        = m_data.reset;
    float maxPvOut     = m_data.maxPvOut;
    int   missingCount = 0;

    // Parse Fields
    gain = src["gain"] | -1.0F;
    if ( Cpl::Math::areFloatsEqual( gain, -1.0F ) )
    {
        missingCount++;
        gain = m_data.gain;
    }
    reset = src["reset"] | -1.0F;
    if ( Cpl::Math::areFloatsEqual( reset, -1.0F ) )
    {
        reset = m_data.reset;
        missingCount++;
    }

    maxPvOut = src["maxPvOut"] | -1.0F;
    if ( Cpl::Math::areFloatsEqual( maxPvOut, -1.0F ) )
    {
        maxPvOut = m_data.maxPvOut;
        missingCount++;
    }

    // Throw an error if NO valid key/value pairs where specified
    if ( missingCount > 0 )
    {
        if ( errorMsg )
        {
            *errorMsg = "Invalid syntax for the 'val' key/value pair";
        }
        return false;
    }

    retSequenceNumber = write( gain, reset, maxPvOut, lockRequest );
    return true;
}

