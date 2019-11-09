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
    Storm::Type::SystemConfig_T* otherPtr = ( Storm::Type::SystemConfig_T* ) otherData;

    // Compare bounds array (must be done brute force since the array contains floats)
    for ( int i=0; i < m_data.totalStages; i++ )
    {
        if ( !Cpl::Math::areFloatsEqual( m_data.stages[i].lowerBound, otherPtr->stages[i].lowerBound ) )
        {
            return false;
        }
        if ( !Cpl::Math::areFloatsEqual( m_data.stages[i].upperBound, otherPtr->stages[i].upperBound ) )
        {
            return false;
        }
        if ( m_data.stages[i].maxIndoorFan != otherPtr->stages[i].maxIndoorFan )
        {
            return false;
        }
        if ( m_data.stages[i].minIndoorFan != otherPtr->stages[i].minIndoorFan )
        {
            return false;
        }
        if ( m_data.stages[i].cph != otherPtr->stages[i].cph )
        {
            return false;
        }
        if ( m_data.stages[i].minOffTime != otherPtr->stages[i].minOffTime )
        {
            return false;
        }
        if ( m_data.stages[i].minOnTime != otherPtr->stages[i].minOnTime )
        {
            return false;
        }
    }

    // Compare non-float fields
    return m_data.allowedOperatingModes == otherPtr->allowedOperatingModes &&
        m_data.indoorUnitType == otherPtr->indoorUnitType &&
        m_data.outdoorUnitType == otherPtr->outdoorUnitType &&
        m_data.numCompressorStages == otherPtr->numCompressorStages &&
        m_data.numIndoorStages == otherPtr->numIndoorStages &&
        m_data.fanContinuousSpeed == otherPtr->fanContinuousSpeed;
}

void MpSystemConfig::copyDataTo_( void* dstData, size_t dstSize ) const noexcept
{
    CPL_SYSTEM_ASSERT( dstSize == sizeof( Storm::Type::SystemConfig_T ) );
    Storm::Type::SystemConfig_T* dstDataPtr = ( Storm::Type::SystemConfig_T* ) dstData;
    *dstDataPtr                             = m_data;
}

void MpSystemConfig::copyDataFrom_( const void* srcData, size_t srcSize ) noexcept
{
    CPL_SYSTEM_ASSERT( srcSize == sizeof( Storm::Type::SystemConfig_T ) );
    Storm::Type::SystemConfig_T* dataSrcPtr = ( Storm::Type::SystemConfig_T* ) srcData;
    m_data                                  = *dataSrcPtr;
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
        valObj["fanCont"]       = m_data.fanContinuousSpeed;

        // Bounds array
        JsonArray  bounds  = valObj.createNestedArray( "stages" );
        for ( int i=0; i < m_data.totalStages; i++ )
        {
            JsonObject elemObj   = bounds.createNestedObject();
            elemObj["stage"]     = i + 1;
            elemObj["lower"]     = ( double) m_data.stages[i].lowerBound;
            elemObj["upper"]     = ( double) m_data.stages[i].upperBound;
            elemObj["minBlower"] = m_data.stages[i].minIndoorFan;
            elemObj["maxBlower"] = m_data.stages[i].maxIndoorFan;
            elemObj["cph"]       = Storm::Type::Cph::_from_integral_unchecked( m_data.stages[i].cph )._to_string();
            elemObj["minOn"]     = m_data.stages[i].minOnTime;
            elemObj["minOff"]    = m_data.stages[i].minOffTime;
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
    num = src["fanCont"] | -1;
    if ( num >= 0 )
    {
        newVal.fanContinuousSpeed = num;
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
    JsonArray stageArray = src["stages"];
    for ( unsigned i=0; i < stageArray.size(); i++ )
    {
        int stageNum = stageArray[i]["stage"];
        if ( stageNum < 1 || stageNum > newVal.totalStages )
        {
            if ( errorMsg )
            {
                errorMsg->format( "Invalid stage number (%d)", stageNum );
            }
            return false;
        }

        newVal.stages[stageNum - 1].lowerBound   = stageArray[i]["lower"] | newVal.stages[stageNum - 1].lowerBound;
        newVal.stages[stageNum - 1].upperBound   = stageArray[i]["upper"] | newVal.stages[stageNum - 1].upperBound;
        newVal.stages[stageNum - 1].minIndoorFan = stageArray[i]["minBlower"] | newVal.stages[stageNum - 1].minIndoorFan;
        newVal.stages[stageNum - 1].maxIndoorFan = stageArray[i]["maxBlower"] | newVal.stages[stageNum - 1].maxIndoorFan;
        newVal.stages[stageNum - 1].minOnTime    = stageArray[i]["minOn"] | newVal.stages[stageNum - 1].minOnTime;
        newVal.stages[stageNum - 1].minOffTime   = stageArray[i]["minOff"] | newVal.stages[stageNum - 1].minOffTime;

        // Enum values
        const char* enumVal = stageArray[i]["cph"];
        if ( enumVal )
        {
            // Convert the text to an enum value
            auto maybeValue = Storm::Type::Cph::_from_string_nothrow( enumVal );
            if ( !maybeValue )
            {
                if ( errorMsg )
                {
                    errorMsg->format( "Invalid enum[%d] value (%s)", i, enumVal );
                }
                return false;
            }

            newVal.stages[stageNum - 1].cph = *maybeValue;
        }
    }


    retSequenceNumber = write( newVal, lockRequest );
    return true;
}


