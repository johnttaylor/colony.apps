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


#include "MpVirtualIduOutputs.h"
#include "Cpl/System/Assert.h"
#include "Cpl/System/FatalError.h"
#include "Cpl/System/Trace.h"
#include <string.h>

#define INVALID_VALUE   9999
#define MAX_OUTPUT      1000

#define SECT_   "Storm::Dm"



///
using namespace Storm::Dm;

///////////////////////////////////////////////////////////////////////////////
MpVirtualIduOutputs::MpVirtualIduOutputs( Cpl::Dm::ModelDatabase& myModelBase, Cpl::Dm::StaticInfo& staticInfo )
    :ModelPointCommon_( myModelBase, &m_data, staticInfo, MODEL_POINT_STATE_VALID )
{
    memset( &m_data, 0, sizeof( m_data ) );
}


///////////////////////////////////////////////////////////////////////////////
uint16_t MpVirtualIduOutputs::setInvalidState( int8_t newInvalidState, LockRequest_T lockRequest ) noexcept
{
    // Set all outputs to off when invalidating the Model Point
    m_modelDatabase.lock_();
    memset( &m_data, 0, sizeof( m_data ) );
    uint16_t result = ModelPointCommon_::setInvalidState( newInvalidState, lockRequest );
    m_modelDatabase.unlock_();
    return result;
}

int8_t MpVirtualIduOutputs::read( Data& dstData, uint16_t* seqNumPtr ) const noexcept
{
    return ModelPointCommon_::read( &dstData, sizeof( Data ), seqNumPtr );
}

uint16_t MpVirtualIduOutputs::write( const Data& srcData, LockRequest_T lockRequest ) noexcept
{
    Data newData = srcData;
    validate( newData );
    return ModelPointCommon_::write( &newData, sizeof( Data ), lockRequest );
}


uint16_t MpVirtualIduOutputs::setFanOuput( uint16_t fanSpeed, LockRequest_T lockRequest ) noexcept
{
    Data newData;
    m_modelDatabase.lock_();

    newData          = m_data;
    newData.fanOuput = fanSpeed;

    uint16_t result = write( newData, lockRequest );
    m_modelDatabase.unlock_();

    return result;
}

uint16_t MpVirtualIduOutputs::setStageOutput( uint8_t stageIndex, uint16_t stageOutput, LockRequest_T lockRequest ) noexcept
{
    // Trap out-of-range stage index value
    if ( stageIndex >= STORM_MAX_INDOOR_HEATING_STAGES )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "MpVirtualIduOutputs::MpVirtualIduOutputs() Invalid stage index=%d (num stages=%d)", stageIndex, OPTION_STORM_MAX_COOLING_STAGES ) );
        return getSequenceNumber();
    }

    Data newData;
    m_modelDatabase.lock_();

    newData                          = m_data;
    newData.stageOutputs[stageIndex] = stageOutput;

    uint16_t result = write( newData );
    m_modelDatabase.unlock_();

    return result;
}

uint16_t MpVirtualIduOutputs::setSafeAllOff( LockRequest_T lockRequest ) noexcept
{
    Data newData;
    m_modelDatabase.lock_();

    newData              = m_data;
    newData.fanOuput     = 0;
    memset( &( newData.stageOutputs ), 0, sizeof( newData.stageOutputs ) );

    uint16_t result = write( newData, lockRequest );
    m_modelDatabase.unlock_();

    return result;
}


uint16_t MpVirtualIduOutputs::readModifyWrite( Client& callbackClient, LockRequest_T lockRequest )
{
    return ModelPointCommon_::readModifyWrite( callbackClient, lockRequest );
}

void MpVirtualIduOutputs::attach( Observer& observer, uint16_t initialSeqNumber ) noexcept
{
    ModelPointCommon_::attach( observer, initialSeqNumber );
}

void MpVirtualIduOutputs::detach( Observer& observer ) noexcept
{
    ModelPointCommon_::detach( observer );
}

bool MpVirtualIduOutputs::isDataEqual_( const void* otherData ) const noexcept
{
    return  memcmp( &m_data, otherData, sizeof( m_data ) ) == 0;
}

void MpVirtualIduOutputs::copyDataTo_( void* dstData, size_t dstSize ) const noexcept
{
    CPL_SYSTEM_ASSERT( dstSize == sizeof( Data ) );
    Data* dstDataPtr   = ( Data*) dstData;
    *dstDataPtr        = m_data;
}

void MpVirtualIduOutputs::copyDataFrom_( const void* srcData, size_t srcSize ) noexcept
{
    CPL_SYSTEM_ASSERT( srcSize == sizeof( Data ) );
    Data* dataSrcPtr   = ( Data*) srcData;
    m_data             = *dataSrcPtr;
}


///////////////////////////////////////////////////////////////////////////////
const char* MpVirtualIduOutputs::getTypeAsText() const noexcept
{
    return "Storm::Dm::MpVirtualIduOutputs";
}

size_t MpVirtualIduOutputs::getSize() const noexcept
{
    return sizeof( Data );
}

size_t MpVirtualIduOutputs::getInternalDataSize_() const noexcept
{
    return sizeof( Data );
}


const void* MpVirtualIduOutputs::getImportExportDataPointer_() const noexcept
{
    return &m_data;
}

bool MpVirtualIduOutputs::toJSON( char* dst, size_t dstSize, bool& truncated, bool verbose ) noexcept
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
        valObj["fan"]      = m_data.fanOuput;

        JsonArray  arrayStages = valObj.createNestedArray( "stages" );
        for ( int i=0; i < STORM_MAX_INDOOR_HEATING_STAGES; i++ )
        {
            JsonObject elemObj  = arrayStages.createNestedObject();
            elemObj["stage"]    = i + 1;
            elemObj["capacity"] = m_data.stageOutputs[i];
        }
    }

    // End the conversion
    endJSON( dst, dstSize, truncated, verbose );

    m_modelDatabase.unlock_();
    return true;
}


bool MpVirtualIduOutputs::fromJSON_( JsonVariant& src, LockRequest_T lockRequest, uint16_t& retSequenceNumber, Cpl::Text::String* errorMsg ) noexcept
{
    Data updatedData = m_data;

    // Fan output
    uint16_t fan = src["fan"] | INVALID_VALUE;
    if ( fan != INVALID_VALUE )
    {
        updatedData.fanOuput = fan;
    }

    // Output stages
    JsonArray stages = src["stages"];
    for ( unsigned i=0; i < stages.size(); i++ )
    {
        int stageNum = stages[i]["stage"];
        if ( stageNum < 1 || stageNum > STORM_MAX_INDOOR_HEATING_STAGES )
        {
            if ( errorMsg )
            {
                errorMsg->format( "Invalid indoor stage number (%d)", stageNum );
            }
            return false;
        }

        updatedData.stageOutputs[stageNum - 1] = stages[i]["capacity"] | updatedData.stageOutputs[stageNum - 1];
    }

    retSequenceNumber = write( updatedData, lockRequest );
    return true;
}

void MpVirtualIduOutputs::validate( Data& newValues ) const noexcept
{
    if ( newValues.fanOuput > MAX_OUTPUT )
    {
        newValues.fanOuput = MAX_OUTPUT;
    }
    for ( int i=0; i < STORM_MAX_INDOOR_HEATING_STAGES; i++ )
    {
        if ( newValues.stageOutputs[i] > MAX_OUTPUT )
        {
            newValues.stageOutputs[i] = MAX_OUTPUT;
        }
    }
}
