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


#include "MpIduConfig.h"
#include "Storm/Constants.h"
#include "Cpl/System/Assert.h"
#include "Cpl/System/FatalError.h"
#include "Cpl/Math/real.h"
#include <string.h>

///
using namespace Storm::Dm;


///////////////////////////////////////////////////////////////////////////////

MpIduConfig::MpIduConfig( Cpl::Dm::ModelDatabase& myModelBase, Cpl::Dm::StaticInfo& staticInfo, Storm::Type::IduType unitType, bool hasVspMotor, uint16_t numHeatingStages )
    : ModelPointCommon_( myModelBase, &m_data, staticInfo, MODEL_POINT_STATE_VALID )
    , m_data( { unitType, hasVspMotor, numHeatingStages } )
{
}

///////////////////////////////////////////////////////////////////////////////
uint16_t MpIduConfig::setInvalidState( int8_t newInvalidState, LockRequest_T lockRequest ) noexcept
{
    m_modelDatabase.lock_();

    // Reset the default values when being invalidated. This ensure proper 
    // behavior when just updating a single field.
    m_data.type             = OPTION_STORM_DM_IDU_CONFIG_DEFAULT_IDUTYPE;
    m_data.hasVspMotor      = OPTION_STORM_DM_IDU_CONFIG_DEFAULT_VSPMOTOR;
    m_data.numHeatingStages = OPTION_STORM_DM_IDU_CONFIG_DEFAULT_NUM_STAGES;

    // Return the sequence number
    uint16_t result  = ModelPointCommon_::setInvalidState( newInvalidState, lockRequest );
    m_modelDatabase.unlock_();
    return result;
}

int8_t MpIduConfig::read( Data& configuration, uint16_t* seqNumPtr ) const noexcept
{
    return ModelPointCommon_::read( &configuration, sizeof( Data ), seqNumPtr );
}

uint16_t MpIduConfig::write( Data& newConfiguration, LockRequest_T lockRequest ) noexcept
{
    // Enforce limits
    validate( newConfiguration );
    return ModelPointCommon_::write( &newConfiguration, sizeof( Data ), lockRequest );
}

uint16_t MpIduConfig::writeType( Storm::Type::IduType newUnitType, LockRequest_T lockRequest ) noexcept
{
    m_modelDatabase.lock_();

    Data     src    = { newUnitType, m_data.hasVspMotor, m_data.numHeatingStages };
    validate( src );
    uint16_t result = ModelPointCommon_::write( &src, sizeof( Data ), lockRequest );

    m_modelDatabase.unlock_();
    return result;
}

uint16_t MpIduConfig::writeFanMotor( bool hasVspMotor, LockRequest_T lockRequest ) noexcept
{
    m_modelDatabase.lock_();

    Data     src    = { m_data.type, hasVspMotor, m_data.numHeatingStages };
    validate( src );
    uint16_t result = ModelPointCommon_::write( &src, sizeof( Data ), lockRequest );

    m_modelDatabase.unlock_();
    return result;
}

uint16_t MpIduConfig::writeHeatingStages( uint16_t numHeatingStages, LockRequest_T lockRequest ) noexcept
{
    m_modelDatabase.lock_();

    Data     src    = { m_data.type, m_data.hasVspMotor, numHeatingStages };
    validate( src );
    uint16_t result = ModelPointCommon_::write( &src, sizeof( Data ), lockRequest );

    m_modelDatabase.unlock_();
    return result;
}


uint16_t MpIduConfig::readModifyWrite( Client & callbackClient, LockRequest_T lockRequest )
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

void MpIduConfig::attach( Observer & observer, uint16_t initialSeqNumber ) noexcept
{
    ModelPointCommon_::attach( observer, initialSeqNumber );
}

void MpIduConfig::detach( Observer & observer ) noexcept
{
    ModelPointCommon_::detach( observer );
}

bool MpIduConfig::isDataEqual_( const void* otherData ) const noexcept
{
    Data* otherDataPtr = ( Data*) otherData;

    return otherDataPtr->hasVspMotor == m_data.hasVspMotor &&
        otherDataPtr->type == m_data.type &&
        otherDataPtr->numHeatingStages == m_data.numHeatingStages;

}

void MpIduConfig::copyDataTo_( void* dstData, size_t dstSize ) const noexcept
{
    CPL_SYSTEM_ASSERT( dstSize == sizeof( Data ) );
    Data* dstDataPtr   = ( Data*) dstData;
    *dstDataPtr        = m_data;
}

void MpIduConfig::copyDataFrom_( const void* srcData, size_t srcSize ) noexcept
{
    CPL_SYSTEM_ASSERT( srcSize == sizeof( Data ) );
    Data* dataSrcPtr   = ( Data*) srcData;
    m_data             = *dataSrcPtr;
}


///////////////////////////////////////////////////////////////////////////////
const char* MpIduConfig::getTypeAsText() const noexcept
{
    return "Storm::Dm::MpIduConfig";
}

size_t MpIduConfig::getSize() const noexcept
{
    return sizeof( Data );
}

size_t MpIduConfig::getInternalDataSize_() const noexcept
{
    return sizeof( Data );
}


const void* MpIduConfig::getImportExportDataPointer_() const noexcept
{
    return &m_data;
}

bool MpIduConfig::toJSON( char* dst, size_t dstSize, bool& truncated, bool verbose ) noexcept
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
        Storm::Type::IduType type = Storm::Type::IduType::_from_integral_unchecked( m_data.type );
        valObj["type"]            = type._to_string();
        valObj["vspMotor"]        = m_data.hasVspMotor;
        valObj["numHeat"]         = m_data.numHeatingStages;
    }

    // End the conversion
    endJSON( dst, dstSize, truncated, verbose );

    m_modelDatabase.unlock_();
    return true;
}

bool MpIduConfig::fromJSON_( JsonVariant & src, LockRequest_T lockRequest, uint16_t & retSequenceNumber, Cpl::Text::String * errorMsg ) noexcept
{
    Data newVal       = { m_data.type, m_data.hasVspMotor, m_data.numHeatingStages };
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
        auto maybeValue = Storm::Type::IduType::_from_string_nothrow( enumVal );
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

    // Parse Fan Motor option
    bool checkForError = src["vspMotor"] | false;
    bool newValue      = src["vspMotor"] | true;
    if ( newValue == true && checkForError == false )
    {
        missingCount++;
    }
    else
    {
        newVal.hasVspMotor = newValue;
    }

    // Parse number of stages
    int stages = src["numHeat"] | -1;
    if ( stages < 0 )
    {
        missingCount++;
    }
    else
    {
        newVal.numHeatingStages = ( uint16_t) stages;
    }

    // Throw an error if NO valid key/value pairs where specified
    if ( missingCount == 3 )
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

bool MpIduConfig::validate( Data& values ) const noexcept
{
    bool modified = false;

    if ( values.type == Storm::Type::IduType::eAIR_HANDLER )
    {
        if ( values.numHeatingStages > OPTION_STORM_MAX_ELECTRIC_HEATING_STAGES )
        {
            values.numHeatingStages = OPTION_STORM_MAX_ELECTRIC_HEATING_STAGES;
            modified                = true;
        }
    }
    else if ( values.type == Storm::Type::IduType::eFURNACE )
    {
        if ( values.numHeatingStages > OPTION_STORM_MAX_FURNACE_HEATING_STAGES )
        {
            values.numHeatingStages = OPTION_STORM_MAX_FURNACE_HEATING_STAGES;
            modified                = true;
        }
    }
    else
    {
        if ( values.numHeatingStages > 0 )
        {
            values.numHeatingStages = 0;
            modified                = true;
        }
    }

    return modified;
}
