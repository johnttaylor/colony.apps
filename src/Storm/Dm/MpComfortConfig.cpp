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


#include "MpComfortConfig.h"
#include "Storm/Component/DutyCycle.h"
#include "Cpl/System/Assert.h"
#include "Cpl/System/FatalError.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Math/real.h"
#include <string.h>

#define SECT_   "Storm::Dm"

///
using namespace Storm::Dm;

static void setDefaults( MpComfortConfig::Parameters_T parms[], uint8_t numStages )
{
    for ( uint8_t i=0; i < numStages; i++ )
    {
        parms[i].cph        = Storm::Type::Cph::e3CPH;
        parms[i].minOffTime = 5 * 60 * 1000;
        parms[i].minOnTime  = 5 * 60 * 1000;
    }
}

///////////////////////////////////////////////////////////////////////////////

MpComfortConfig::MpComfortConfig( Cpl::Dm::ModelDatabase& myModelBase, Cpl::Dm::StaticInfo& staticInfo )
    : ModelPointCommon_( myModelBase, &m_data, staticInfo, MODEL_POINT_STATE_VALID )
{
    memset( &m_data, 0, sizeof( m_data ) ); // Set all potential 'pad bytes' to zero so memcmp() will work correctly
    setDefaults( m_data.cooling, OPTION_STORM_MAX_COOLING_STAGES );
    setDefaults( m_data.heating, OPTION_STORM_MAX_HEATING_STAGES );
}

///////////////////////////////////////////////////////////////////////////////
uint16_t MpComfortConfig::setInvalidState( int8_t newInvalidState, LockRequest_T lockRequest ) noexcept
{
    m_modelDatabase.lock_();

    // Reset the default values when being invalidated. This ensure proper 
    // behavior when just updating a single field.
    setDefaults( m_data.cooling, OPTION_STORM_MAX_COOLING_STAGES );
    setDefaults( m_data.heating, OPTION_STORM_MAX_HEATING_STAGES );

    // Return the sequence number
    uint16_t result  = ModelPointCommon_::setInvalidState( newInvalidState, lockRequest );
    m_modelDatabase.unlock_();
    return result;
}

int8_t MpComfortConfig::read( Data& configuration, uint16_t* seqNumPtr ) const noexcept
{
    return ModelPointCommon_::read( &configuration, sizeof( Data ), seqNumPtr );
}

uint16_t MpComfortConfig::write( Data& newConfiguration, LockRequest_T lockRequest ) noexcept
{
    validate( newConfiguration );
    return ModelPointCommon_::write( &newConfiguration, sizeof( Data ), lockRequest );
}

uint16_t MpComfortConfig::writeCoolingStage( uint8_t stageIndex, Parameters_T newStageParameters, LockRequest_T lockRequest ) noexcept
{
    // DO NOTHING if the stage index is out of bounds
    if ( stageIndex >= OPTION_STORM_MAX_COOLING_STAGES )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "MpComfortConfig::writeCoolingStage() Invalid stage index=%d (num stages=%d)", stageIndex, OPTION_STORM_MAX_COOLING_STAGES ) );
        return getSequenceNumber();
    }

    m_modelDatabase.lock_();

    Data src = m_data;
    src.cooling[stageIndex] = newStageParameters;
    uint16_t result = ModelPointCommon_::write( &src, sizeof( Data ), lockRequest );

    m_modelDatabase.unlock_();
    return result;
}

uint16_t MpComfortConfig::writeHeatingStage( uint8_t stageIndex, Parameters_T newStageParameters, LockRequest_T lockRequest ) noexcept
{
    // DO NOTHING if the stage index is out of bounds
    if ( stageIndex >= OPTION_STORM_MAX_HEATING_STAGES )
    {
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "MpComfortConfig::writeHeatingStage() Invalid stage index=%d (num stages=%d)", stageIndex, OPTION_STORM_MAX_HEATING_STAGES ) );
        return getSequenceNumber();
    }
    
    m_modelDatabase.lock_();

    Data src = m_data;
    src.heating[stageIndex] = newStageParameters;
    uint16_t result = ModelPointCommon_::write( &src, sizeof( Data ), lockRequest );

    m_modelDatabase.unlock_();
    return result;
}


uint16_t MpComfortConfig::readModifyWrite( Client & callbackClient, LockRequest_T lockRequest )
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

void MpComfortConfig::attach( Observer & observer, uint16_t initialSeqNumber ) noexcept
{
    ModelPointCommon_::attach( observer, initialSeqNumber );
}

void MpComfortConfig::detach( Observer & observer ) noexcept
{
    ModelPointCommon_::detach( observer );
}

bool MpComfortConfig::isDataEqual_( const void* otherData ) const noexcept
{
    return memcmp( &m_data, otherData, sizeof( m_data ) ) == 0;
}

void MpComfortConfig::copyDataTo_( void* dstData, size_t dstSize ) const noexcept
{
    CPL_SYSTEM_ASSERT( dstSize == sizeof( Data ) );
    Data* dstDataPtr   = ( Data*) dstData;
    *dstDataPtr        = m_data;
}

void MpComfortConfig::copyDataFrom_( const void* srcData, size_t srcSize ) noexcept
{
    CPL_SYSTEM_ASSERT( srcSize == sizeof( Data ) );
    Data* dataSrcPtr   = ( Data*) srcData;
    m_data             = *dataSrcPtr;
}


///////////////////////////////////////////////////////////////////////////////
const char* MpComfortConfig::getTypeAsText() const noexcept
{
    return "Storm::Dm::MpComfortConfig";
}

size_t MpComfortConfig::getSize() const noexcept
{
    return sizeof( Data );
}

size_t MpComfortConfig::getInternalDataSize_() const noexcept
{
    return sizeof( Data );
}


const void* MpComfortConfig::getImportExportDataPointer_() const noexcept
{
    return &m_data;
}

static void buildEntry( JsonObject& obj, MpComfortConfig::Parameters_T& parms, uint8_t index )
{
    Storm::Type::Cph cph = Storm::Type::Cph::_from_integral_unchecked( parms.cph );
    valObj["cph"]        = cph._to_string();
}
bool MpComfortConfig::toJSON( char* dst, size_t dstSize, bool& truncated, bool verbose ) noexcept
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
        JsonArray  arrayCool      = valObj.createNestedArray( "cool" );
        JsonArray  arrayHeat      = valObj.createNestedArray( "heat" );
        Storm::Type::OduType type = Storm::Type::OduType::_from_integral_unchecked( m_data.type );
        valObj["type"]            = type._to_string();
        valObj["numStages"]       = m_data.numStages;
    }

    // End the conversion
    endJSON( dst, dstSize, truncated, verbose );

    m_modelDatabase.unlock_();
    return true;
}

bool MpComfortConfig::fromJSON_( JsonVariant & src, LockRequest_T lockRequest, uint16_t & retSequenceNumber, Cpl::Text::String * errorMsg ) noexcept
{
#if 0
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
#endif
    return true;
}

bool MpComfortConfig::validate( Data& values ) const noexcept
{
    bool modified = false;

    
    for ( uint8_t i=0; i < OPTION_STORM_MAX_COOLING_STAGES; i++ )
    {
        modified |= validate( values.cooling[i] );
    }

    for ( uint8_t i=0; i < OPTION_STORM_MAX_HEATING_STAGES; i++ )
    {
        modified |= validate( values.heating[i] );
    }

    return modified;
}

bool MpComfortConfig::validate( Parameters_T& src ) const noexcept
{
    bool modified = false;

    if ( src.cph >= Storm::Type::Cph::eNUM_OPTIONS || src.cph < Storm::Type::Cph::e2CPH )
    {
        modified = true;
        src.cph  = Storm::Type::Cph::e3CPH;
    }

    uint32_t limit = Storm::Component::DutyCycle::getMaximumMinOffTime( Storm::Type::Cph::_from_integral_unchecked(src.cph) );
    if ( src.minOffTime > limit )
    {
        src.minOffTime = limit;
        modified       = true;
    }
    
    limit = Storm::Component::DutyCycle::getMaximumMinOnTime( Storm::Type::Cph::_from_integral_unchecked( src.cph ) );
    if ( src.minOnTime > limit )
    {
        src.minOnTime = limit;
        modified      = true;
    }

    return modified;
}


