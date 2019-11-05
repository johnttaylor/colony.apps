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


#include "MpSystemConfig.h"
#include "Storm/Utils/DutyCycle.h"
#include "Cpl/System/Assert.h"
#include "Cpl/System/FatalError.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Math/real.h"
#include <string.h>

#define SECT_   "Storm::Dm"

///
using namespace Storm::Dm;



///////////////////////////////////////////////////////////////////////////////

MpSystemConfig::MpSystemConfig( Cpl::Dm::ModelDatabase& myModelBase, Cpl::Dm::StaticInfo& staticInfo )
    : ModelPointCommon_( myModelBase, &m_data, staticInfo, OPTION_CPL_RTE_MODEL_POINT_STATE_INVALID )
{
    memset( &m_data, 0, sizeof( m_data ) ); // Set all potential 'pad bytes' to zero so memcmp() will work correctly
}

///////////////////////////////////////////////////////////////////////////////
int8_t MpSystemConfig::read( Storm::Type::SystemConfig_T& configuration, uint16_t* seqNumPtr ) const noexcept
{
    return ModelPointCommon_::read( &configuration, sizeof( Storm::Type::SystemConfig_T ), seqNumPtr );
}

uint16_t MpSystemConfig::write( Storm::Type::SystemConfig_T& newConfiguration, LockRequest_T lockRequest ) noexcept
{
    return ModelPointCommon_::write( &newConfiguration, sizeof( Storm::Type::SystemConfig_T ), lockRequest );
}




uint16_t MpSystemConfig::readModifyWrite( Client & callbackClient, LockRequest_T lockRequest )
{
    return ModelPointCommon_::readModifyWrite( callbackClient, lockRequest );
}

void MpSystemConfig::attach( Observer & observer, uint16_t initialSeqNumber ) noexcept
{
    ModelPointCommon_::attach( observer, initialSeqNumber );
}

void MpSystemConfig::detach( Observer & observer ) noexcept
{
    ModelPointCommon_::detach( observer );
}

bool MpSystemConfig::isDataEqual_( const void* otherData ) const noexcept
{
    return memcmp( &m_data, otherData, sizeof( m_data ) ) == 0;
}

void MpSystemConfig::copyDataTo_( void* dstData, size_t dstSize ) const noexcept
{
    CPL_SYSTEM_ASSERT( dstSize == sizeof( Storm::Type::SystemConfig_T ) );
    Storm::Type::SystemConfig_T* dstDataPtr   = ( Storm::Type::SystemConfig_T*) dstData;
    *dstDataPtr        = m_data;
}

void MpSystemConfig::copyDataFrom_( const void* srcData, size_t srcSize ) noexcept
{
    CPL_SYSTEM_ASSERT( srcSize == sizeof( Storm::Type::SystemConfig_T ) );
    Storm::Type::SystemConfig_T* dataSrcPtr   = ( Storm::Type::SystemConfig_T*) srcData;
    m_data             = *dataSrcPtr;
}


///////////////////////////////////////////////////////////////////////////////
const char* MpSystemConfig::getTypeAsText() const noexcept
{
    return "Storm::Dm::MpSystemConfig";
}

size_t MpSystemConfig::getSize() const noexcept
{
    return sizeof( Storm::Type::SystemConfig_T );
}

size_t MpSystemConfig::getInternalDataSize_() const noexcept
{
    return sizeof( Storm::Type::SystemConfig_T );
}


const void* MpSystemConfig::getImportExportDataPointer_() const noexcept
{
    return &m_data;
}

static void buildEntry( JsonObject& obj, float lowerBound, float upperBound, uint8_t index )
{
    obj["stage"]         = index + 1;
    obj["lower"]         = lowerBound;
    obj["upper"]         = upperBound;
}

bool MpSystemConfig::toJSON( char* dst, size_t dstSize, bool& truncated, bool verbose ) noexcept
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
        // Parameters
        JsonObject valObj       = doc.createNestedObject( "val" );
        valObj["allowedModes"]  = Storm::Type::AllowedOperatingModes::_from_integral_unchecked( m_data.allowedOperatingModes )._to_string();
        valObj["oduType"]       = Storm::Type::OduType::_from_integral_unchecked( m_data.outdoorUnitType )._to_string();
        valObj["iduType"]       = Storm::Type::IduType::_from_integral_unchecked( m_data.indoorUnitType )._to_string();
        valObj["numCompStages"] = m_data.numCompressorStages;
        valObj["numIdStages"]   = m_data.numIndoorStages;
        valObj["totalStages"]   = m_data.totalStages;

        // Bounds array
        JsonArray  bounds  = valObj.createNestedArray( "pvBounds" );
        for ( int i=0; i < m_data.totalStages; i++ )
        {
            JsonObject elemObj = bounds.createNestedObject();
            elemObj["stage"]   = i + 1;
            elemObj["lower"]   = m_data.pvBounds[i].lowerBound;
            elemObj["upper"]   = m_data.pvBounds[i].upperBound;
        }
    }

    // End the conversion
    endJSON( dst, dstSize, truncated, verbose );

    m_modelDatabase.unlock_();
    return true;
}

bool MpSystemConfig::fromJSON_( JsonVariant & src, LockRequest_T lockRequest, uint16_t & retSequenceNumber, Cpl::Text::String * errorMsg ) noexcept
{
    Storm::Type::SystemConfig_T newVal = m_data;

    // Numeric values
    int num = src["numCompStages"] | -1;
    if ( num >= 0 )
    {
        newVal.numCompressorStages = num;
    }
    num = src["numIdStages"] | -1;
    if ( num >= 0 )
    {
        newVal.numIndoorStages = num;
    }

    num = src["totalStages"] | -1;
    if ( num >= 0 )
    {
        newVal.totalStages = num;
    }

    // Enum values
    const char* enumVal = src["allowedModes"];
    if ( enumVal )
    {
        // Convert the text to an enum value
        auto maybeValue = Storm::Type::AllowedOperatingModes::_from_string_nothrow( enumVal );
        if ( !maybeValue )
        {
            if ( errorMsg )
            {
                errorMsg->format( "Invalid enum value (%s)", enumVal );
            }
            return false;
        }

        newVal.allowedOperatingModes = *maybeValue;
    }
    enumVal = src["iduType"];
    if ( enumVal )
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

        newVal.indoorUnitType = *maybeValue;
    }
    enumVal = src["oduType"];
    if ( enumVal )
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

        newVal.outdoorUnitType = *maybeValue;
    }


    // PV Bounds
    JsonArray boundsArray = src["pvBounds"];
    for ( unsigned i=0; i < boundsArray.size(); i++ )
    {
        int stageNum = boundsArray[i]["stage"];
        if ( stageNum < 1 || stageNum > newVal.totalStages )
        {
            if ( errorMsg )
            {
                errorMsg->format( "Invalid bounds stage number (%d)", stageNum );
            }
            return false;
        }

        newVal.pvBounds[stageNum - 1].lowerBound = boundsArray[i]["lower"] | newVal.pvBounds[stageNum - 1].lowerBound;
        newVal.pvBounds[stageNum - 1].upperBound = boundsArray[i]["upper"] | newVal.pvBounds[stageNum - 1].upperBound;
     }


    retSequenceNumber = write( newVal, lockRequest );
    return true;
}


