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


#include "MpOduConfig.h"
#include "Cpl/System/Assert.h"
#include "Cpl/System/FatalError.h"
#include "Cpl/Math/real.h"
#include <string.h>

///
using namespace Storm::Dm;


///////////////////////////////////////////////////////////////////////////////

MpOduConfig::MpOduConfig( Cpl::Dm::ModelDatabase& myModelBase, Cpl::Dm::StaticInfo& staticInfo, Storm::Type::OduType unitType, uint16_t numCompressorStages )
    : ModelPointCommon_( myModelBase, &m_data, staticInfo, MODEL_POINT_STATE_VALID )
    , m_data( { unitType, numCompressorStages } )
{
}

///////////////////////////////////////////////////////////////////////////////
uint16_t MpOduConfig::setInvalidState( int8_t newInvalidState, LockRequest_T lockRequest ) noexcept
{
    m_modelDatabase.lock_();

    // Reset the default values when being invalidated. This ensure proper 
    // behavior when just updating a single field.
    m_data.type      = OPTION_STORM_DM_ODU_CONFIG_DEFAULT_ODUTYPE;
    m_data.numStages = OPTION_STORM_DM_ODU_CONFIG_DEFAULT_NUM_STAGES;

    // Return the sequence number
    uint16_t result  = ModelPointCommon_::setInvalidState( newInvalidState, lockRequest );
    m_modelDatabase.unlock_();
    return result;
}

int8_t MpOduConfig::read( Data& configuration, uint16_t* seqNumPtr ) const noexcept
{
    return ModelPointCommon_::read( &configuration, sizeof( Data ), seqNumPtr );
}

uint16_t MpOduConfig::write( Data& newConfiguration, LockRequest_T lockRequest ) noexcept
{
    validate( newConfiguration );
    return ModelPointCommon_::write( &newConfiguration, sizeof( Data ), lockRequest );
}

uint16_t MpOduConfig::writeType( Storm::Type::OduType newUnitType, LockRequest_T lockRequest ) noexcept
{
    m_modelDatabase.lock_();

    Data     src    = { newUnitType, m_data.numStages };
    validate( src );
    uint16_t result = ModelPointCommon_::write( &src, sizeof( Data ), lockRequest );

    m_modelDatabase.unlock_();
    return result;
}

uint16_t MpOduConfig::writeCompressorStages( uint16_t numStages, LockRequest_T lockRequest ) noexcept
{
    m_modelDatabase.lock_();

    Data     src    = { m_data.type, numStages };
    validate( src );
    uint16_t result = ModelPointCommon_::write( &src, sizeof( Data ), lockRequest );

    m_modelDatabase.unlock_();
    return result;
}


uint16_t MpOduConfig::readModifyWrite( Client & callbackClient, LockRequest_T lockRequest )
{
    m_modelDatabase.lock_();
    uint16_t result = ModelPointCommon_::readModifyWrite( callbackClient, lockRequest );

    // Ensure the limits are enforced
    // NOTE: In theory this method only needs to be called when there was a
    //       operation.  However, if the data was just 'read' or invalidated,
    //       enforcing the range limits is either a NOP or a don't care
    //       respectively.
    validate( m_data );

    m_modelDatabase.unlock_();
    return result;
}

void MpOduConfig::attach( Observer & observer, uint16_t initialSeqNumber ) noexcept
{
    ModelPointCommon_::attach( observer, initialSeqNumber );
}

void MpOduConfig::detach( Observer & observer ) noexcept
{
    ModelPointCommon_::detach( observer );
}

bool MpOduConfig::isDataEqual_( const void* otherData ) const noexcept
{
    Data* otherDataPtr = ( Data*) otherData;

    return otherDataPtr->type == m_data.type &&
        otherDataPtr->numStages == m_data.numStages;

}

void MpOduConfig::copyDataTo_( void* dstData, size_t dstSize ) const noexcept
{
    CPL_SYSTEM_ASSERT( dstSize == sizeof( Data ) );
    Data* dstDataPtr   = ( Data*) dstData;
    *dstDataPtr        = m_data;
}

void MpOduConfig::copyDataFrom_( const void* srcData, size_t srcSize ) noexcept
{
    CPL_SYSTEM_ASSERT( srcSize == sizeof( Data ) );
    Data* dataSrcPtr   = ( Data*) srcData;
    m_data             = *dataSrcPtr;
}


///////////////////////////////////////////////////////////////////////////////
const char* MpOduConfig::getTypeAsText() const noexcept
{
    return "Storm::Dm::MpOduConfig";
}

size_t MpOduConfig::getSize() const noexcept
{
    return sizeof( Data );
}

size_t MpOduConfig::getInternalDataSize_() const noexcept
{
    return sizeof( Data );
}


const void* MpOduConfig::getImportExportDataPointer_() const noexcept
{
    return &m_data;
}

bool MpOduConfig::toJSON( char* dst, size_t dstSize, bool& truncated, bool verbose ) noexcept
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
        JsonObject valObj         = doc.createNestedObject( "val" );
        Storm::Type::OduType type = Storm::Type::OduType::_from_integral_unchecked( m_data.type );
        valObj["type"]            = type._to_string();
        valObj["numStages"]       = m_data.numStages;
    }

    // End the conversion
    endJSON( dst, dstSize, truncated, verbose );

    m_modelDatabase.unlock_();
    return true;
}

bool MpOduConfig::fromJSON_( JsonVariant & src, LockRequest_T lockRequest, uint16_t & retSequenceNumber, Cpl::Text::String * errorMsg ) noexcept
{
    Data newVal       = { m_data.type, m_data.numStages };
    int  missingCount = 0;

    // Parse Type field
    const char* enumVal = src["type"];
    if ( !enumVal )
    {
        missingCount++;
    }
    else
    {
        // Convert the text to an enum value
        auto maybeValue = Storm::Type::OduType::_from_string_nothrow( enumVal );
        if ( !maybeValue )
        {
            if ( errorMsg )
            {
                errorMsg->format( "Invalid enum value (%s)", enumVal );
            }
            return false;
        }

        newVal.type = *maybeValue;
    }

    // Parse number of stages
    int stages = src["numStages"] | -1;
    if ( stages < 0 )
    {
        missingCount++;
    }
    else
    {
        newVal.numStages = ( uint16_t) stages;
    }

    // Throw an error if NO valid key/value pairs where specified
    if ( missingCount == 2 )
    {
        if ( errorMsg )
        {
            *errorMsg = "Invalid syntax for the 'val' key/value pair";
        }
        return false;
    }

    retSequenceNumber = write( newVal, lockRequest );
    return true;
}

bool MpOduConfig::validate( Data& values ) const noexcept
{
    bool modified = false;

    if ( values.numStages > OPTION_STORM_DM_ODU_CONFIG_MAX_COMPRESSOR_STAGES )
    {
        values.numStages = OPTION_STORM_DM_ODU_CONFIG_MAX_COMPRESSOR_STAGES;
        modified         = true;
    }

    return modified;
}
