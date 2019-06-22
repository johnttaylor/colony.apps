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

#include "Catch/catch.hpp"
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Api.h"
#include "Cpl/Text/FString.h"
#include "Cpl/Text/DString.h"
#include "Cpl/Math/real.h"
#include "Cpl/Dm/ModelDatabase.h"
#include "Storm/Dm/MpOduConfig.h"
#include "common.h"
#include <string.h>



using namespace Storm::Dm;
static Cpl::Dm::MailboxServer     t1Mbox_;

#define STRCMP(a,b)               (strcmp(a,b) == 0)

/////////////////////////////////////////////////////////////////
namespace {
class Rmw : public MpOduConfig::Client
{
public:
    ///
    int                                         m_callbackCount;
    ///
    Cpl::Dm::ModelPoint::RmwCallbackResult_T    m_returnResult;
    ///
    uint16_t                                    m_newStages;

public:
    ///
    Rmw() :m_callbackCount( 0 ), m_returnResult( Cpl::Dm::ModelPoint::eNO_CHANGE ), m_newStages( 0 ) {}
    ///
    Cpl::Dm::ModelPoint::RmwCallbackResult_T callback( MpOduConfig::Data& data, int8_t validState ) noexcept
    {
        m_callbackCount++;
        if ( m_returnResult != Cpl::Dm::ModelPoint::eNO_CHANGE )
        {
            data.numStages= m_newStages;
        }
        return m_returnResult;
    }
};
}; // end Anonymous namespace

////////////////////////////////////////////////////////////////////////////////

// Allocate/create my Model Database
static Cpl::Dm::ModelDatabase   modelDb_( "ignoreThisParameter_usedToInvokeTheStaticConstructor" );

// Allocate my Model Points
static Cpl::Dm::StaticInfo      info_mp_apple_( "APPLE" );
static MpOduConfig              mp_apple_( modelDb_, info_mp_apple_ );

static Cpl::Dm::StaticInfo      info_mp_orange_( "ORANGE" );
static MpOduConfig              mp_orange_( modelDb_, info_mp_orange_, Storm::Type::OduType::eHP,1 );


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "MP Outdoor Unit Config" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();

    SECTION( "ReadWrite" )
    {
        CPL_SYSTEM_TRACE_SCOPE( SECT_, "READWRITE test" );

        // Read
        MpOduConfig::Data value;
        uint16_t          seqNum;
        int8_t            valid = mp_orange_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.numStages == 1 );
        REQUIRE( value.type == Storm::Type::OduType::eHP );
        valid = mp_apple_.read( value, &seqNum );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.numStages == 1 );
        REQUIRE( value.type == Storm::Type::OduType::eAC );

        // Write
        value = { Storm::Type::OduType::eHP, 0 };
        uint16_t seqNum2 = mp_apple_.write( value );
        valid = mp_apple_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.numStages == 0 );
        REQUIRE( value.type == Storm::Type::OduType::eHP );
        REQUIRE( seqNum + 1 == seqNum2 );

        // Write out-of-range
        value = { Storm::Type::OduType::eHP, 10 };
        seqNum = mp_apple_.write( value );
        valid = mp_apple_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.numStages == 1 );
        REQUIRE( value.type == Storm::Type::OduType::eHP );
        REQUIRE( seqNum == seqNum2 + 1 );

        // Read-Modify-Write with Lock
        Rmw callbackClient;
        callbackClient.m_callbackCount  = 0;
        callbackClient.m_newStages      = 0;
        callbackClient.m_returnResult   = Cpl::Dm::ModelPoint::eCHANGED;
        mp_apple_.readModifyWrite( callbackClient, Cpl::Dm::ModelPoint::eLOCK );
        valid         = mp_apple_.read( value );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        bool locked = mp_apple_.isLocked();
        REQUIRE( locked == true );
        REQUIRE( value.numStages == 0 );
        REQUIRE( value.type == Storm::Type::OduType::eHP );
        REQUIRE( callbackClient.m_callbackCount == 1 );

        // Read-Modify-Write with out-of-range values
        callbackClient.m_callbackCount  = 0;
        callbackClient.m_newStages      = 2;
        callbackClient.m_returnResult   = Cpl::Dm::ModelPoint::eCHANGED;
        mp_apple_.readModifyWrite( callbackClient, Cpl::Dm::ModelPoint::eUNLOCK );
        valid         = mp_apple_.read( value );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.numStages == 1 );
        REQUIRE( value.type == Storm::Type::OduType::eHP );
        REQUIRE( callbackClient.m_callbackCount == 1 );

        // Invalidate with Unlock
        mp_apple_.setInvalidState( 112, Cpl::Dm::ModelPoint::eUNLOCK );
        REQUIRE( mp_apple_.isNotValid() == true );
        valid = mp_apple_.getValidState();
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == false );
        REQUIRE( valid == 112 );

        // Single writes
        mp_apple_.writeType( Storm::Type::OduType::eHP );
        valid = mp_apple_.read( value );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.numStages == OPTION_STORM_DM_ODU_CONFIG_DEFAULT_NUM_STAGES );
        REQUIRE( value.type == Storm::Type::OduType::eHP );

        // Single writes
        mp_apple_.writeCompressorStages( 0 );
        valid = mp_apple_.read( value );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.numStages == 0 );
        REQUIRE( value.type == Storm::Type::OduType::eHP );

        // Single writes (out-of-range
        mp_apple_.writeCompressorStages( 3 );
        valid = mp_apple_.read( value );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.numStages == 1 );
        REQUIRE( value.type == Storm::Type::OduType::eHP );
    }

    SECTION( "get" )
    {
        CPL_SYSTEM_TRACE_SCOPE( SECT_, "GET test" );

        // Gets...
        const char* name = mp_apple_.getName();
        REQUIRE( strcmp( name, "APPLE" ) == 0 );
        name = mp_orange_.getName();
        REQUIRE( strcmp( name, "ORANGE" ) == 0 );

        size_t s = mp_apple_.getSize();
        REQUIRE( s == sizeof( MpOduConfig::Data ) );
        s = mp_orange_.getSize();
        REQUIRE( s == sizeof( MpOduConfig::Data ) );

        s = mp_apple_.getExternalSize();
        REQUIRE( s == sizeof( MpOduConfig::Data ) + sizeof( int8_t ) );
        s = mp_orange_.getExternalSize();
        REQUIRE( s == sizeof( MpOduConfig::Data ) + sizeof( int8_t ) );

        const char* mpType = mp_apple_.getTypeAsText();
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "typeText: [%s])", mpType ) );
        REQUIRE( strcmp( mpType, "Storm::Dm::MpOduConfig" ) == 0 );
    }

#define STREAM_BUFFER_SIZE  100

    SECTION( "export/import" )
    {
        CPL_SYSTEM_TRACE_SCOPE( SECT_, "EXPORT/IMPORT test" );

        //  Export/Import Buffer
        uint8_t streamBuffer[STREAM_BUFFER_SIZE];
        REQUIRE( mp_apple_.getExternalSize() <= STREAM_BUFFER_SIZE );


        // Export...
        mp_apple_.setInvalid();
        REQUIRE( mp_apple_.isNotValid() == true );
        uint16_t seqNum  = mp_apple_.getSequenceNumber();
        uint16_t seqNum2 = mp_apple_.getSequenceNumber();
        size_t b = mp_apple_.exportData( streamBuffer, sizeof( streamBuffer ), &seqNum2 );
        REQUIRE( b != 0 );
        REQUIRE( b == mp_apple_.getExternalSize() );
        REQUIRE( seqNum == seqNum2 );

        // Update the MP
        MpOduConfig::Data value = { Storm::Type::OduType::eHP, 0 };
        seqNum = mp_apple_.write( value );
        REQUIRE( seqNum == seqNum2 + 1 );
        int8_t valid;
        valid = mp_apple_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( value.numStages == 0 );
        REQUIRE( value.type == Storm::Type::OduType::eHP );

        // Import...
        b = mp_apple_.importData( streamBuffer, sizeof( streamBuffer ), &seqNum2 );
        REQUIRE( b != 0 );
        REQUIRE( b == mp_apple_.getExternalSize() );
        REQUIRE( seqNum + 1 == seqNum2 );

        // Read import value/state
        valid = mp_apple_.read( value );
        REQUIRE( mp_apple_.isNotValid() == true );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == false );

        // Update the MP
        value = { Storm::Type::OduType::eHP, 1 };
        seqNum = mp_apple_.write( value );
        REQUIRE( seqNum == seqNum2 + 1 );
        valid = mp_apple_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.numStages == 1 );
        REQUIRE( value.type == Storm::Type::OduType::eHP );

        // Export...
        REQUIRE( mp_apple_.isNotValid() == false );
        b = mp_apple_.exportData( streamBuffer, sizeof( streamBuffer ), &seqNum2 );
        REQUIRE( b != 0 );
        REQUIRE( b == mp_apple_.getExternalSize() );
        REQUIRE( seqNum == seqNum2 );

        // Invalidate the MP
        seqNum = mp_apple_.setInvalid();
        REQUIRE( seqNum == seqNum2 + 1 );
        REQUIRE( mp_apple_.isNotValid() == true );

        // Import...
        b = mp_apple_.importData( streamBuffer, sizeof( streamBuffer ), &seqNum2 );
        REQUIRE( b != 0 );
        REQUIRE( b == mp_apple_.getExternalSize() );
        REQUIRE( seqNum + 1 == seqNum2 );

        // Read import value/state
        valid = mp_apple_.read( value );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.numStages == 1 );
        REQUIRE( value.type == Storm::Type::OduType::eHP );
    }

    ///////////////////////////////////////////////////////////////////////////////


    SECTION( "observer" )
    {
        CPL_SYSTEM_TRACE_SCOPE( SECT_, "observer TEST" );

        Cpl::System::Thread* t1 = Cpl::System::Thread::create( t1Mbox_, "T1" );
        AsyncClient<MpOduConfig> viewer1( t1Mbox_, Cpl::System::Thread::getCurrent(), mp_apple_ );

        // Open, write a value, wait for Viewer to see the change, then close
        mp_apple_.removeLock();
        viewer1.open();
        MpOduConfig::Data value = { Storm::Type::OduType::eHP, 0 };
        uint16_t seqNum = mp_apple_.write( value );
        Cpl::System::Thread::wait();
        viewer1.close();
        REQUIRE( viewer1.m_lastSeqNumber == seqNum );

        // Shutdown threads
        t1Mbox_.pleaseStop();
        Cpl::System::Api::sleep( 100 ); // allow time for threads to stop
        REQUIRE( t1->isRunning() == false );
        Cpl::System::Thread::destroy( *t1 );
    }

    ///////////////////////////////////////////////////////////////////////////////
#define MAX_STR_LENG    1024

    SECTION( "toJSON" )
    {
        CPL_SYSTEM_TRACE_SCOPE( SECT_, "TOJSON test" );
        char string[MAX_STR_LENG + 1];
        bool truncated;


        SECTION( "Invalid/terse" )
        {
            // Invalid (Default value)
            mp_apple_.setInvalid();
            mp_apple_.toJSON( string, MAX_STR_LENG, truncated, false );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: terse [%s])", string ) );
            REQUIRE( truncated == false );

            StaticJsonDocument<1024> doc;
            DeserializationError err = deserializeJson( doc, string );
            REQUIRE( err == DeserializationError::Ok );
            REQUIRE( STRCMP( doc["name"], "APPLE" ) );
            REQUIRE( doc["type"].as<char*>() == 0 );
            REQUIRE( doc["seqnum"].as<char*>() == 0 );
            REQUIRE( doc["locked"].as<char*>() == 0 );
            REQUIRE( doc["invalid"] > 0 );
        }

        SECTION( "Invalid" )
        {
            // Invalid (Default value)
            uint16_t seqnum = mp_apple_.setInvalid();
            mp_apple_.toJSON( string, MAX_STR_LENG, truncated );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: [%s])", string ) );
            REQUIRE( truncated == false );

            StaticJsonDocument<1024> doc;
            DeserializationError err = deserializeJson( doc, string );
            REQUIRE( err == DeserializationError::Ok );
            REQUIRE( STRCMP( doc["name"], "APPLE" ) );
            REQUIRE( STRCMP( doc["type"], mp_apple_.getTypeAsText() ) );
            REQUIRE( doc["seqnum"] == seqnum );
            REQUIRE( doc["locked"] == false );
            REQUIRE( doc["invalid"] > 0 );
        }

        SECTION( "Invalid + Locked" )
        {
            mp_apple_.applyLock();
            mp_apple_.toJSON( string, MAX_STR_LENG, truncated );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: [%s])", string ) );

            StaticJsonDocument<1024> doc;
            DeserializationError err = deserializeJson( doc, string );
            REQUIRE( err == DeserializationError::Ok );
            REQUIRE( doc["locked"] == true );
            REQUIRE( doc["invalid"] > 0 );
        }

        SECTION( "Invalid - custom value" )
        {
            mp_apple_.removeLock();
            uint16_t seqnum = mp_apple_.setInvalidState( 100 );
            mp_apple_.toJSON( string, MAX_STR_LENG, truncated );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: [%s])", string ) );

            StaticJsonDocument<1024> doc;
            DeserializationError err = deserializeJson( doc, string );
            REQUIRE( err == DeserializationError::Ok );
            REQUIRE( doc["seqnum"] == seqnum );
            REQUIRE( doc["invalid"] == 100 );
            REQUIRE( doc["locked"] == false );
        }

        SECTION( "Invalid - custom value + locked" )
        {
            // Invalid (custom value) + Locked
            mp_apple_.applyLock();
            mp_apple_.toJSON( string, MAX_STR_LENG, truncated );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: [%s])", string ) );

            StaticJsonDocument<1024> doc;
            DeserializationError err = deserializeJson( doc, string );
            REQUIRE( err == DeserializationError::Ok );
            REQUIRE( doc["invalid"] == 100 );
            REQUIRE( doc["locked"] == true );
        }

        SECTION( "Value" )
        {
            MpOduConfig::Data value = { Storm::Type::OduType::eHP, 1 };
            uint16_t seqnum  = mp_apple_.write( value, Cpl::Dm::ModelPoint::eUNLOCK );
            mp_apple_.toJSON( string, MAX_STR_LENG, truncated );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: [%s])", string ) );

            StaticJsonDocument<1024> doc;
            DeserializationError err = deserializeJson( doc, string );
            REQUIRE( err == DeserializationError::Ok );
            REQUIRE( doc["seqnum"] == seqnum );
            REQUIRE( doc["locked"] == false );
            REQUIRE( doc["invalid"] == 0 );
            JsonObject val = doc["val"];
            REQUIRE( STRCMP( val["type"], "eHP" ) );
            REQUIRE( val["numStages"] == 1 );
        }

        SECTION( "Value + Lock" )
        {
            mp_apple_.applyLock();
            mp_apple_.toJSON( string, MAX_STR_LENG, truncated );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: [%s])", string ) );

            StaticJsonDocument<1024> doc;
            DeserializationError err = deserializeJson( doc, string );
            REQUIRE( err == DeserializationError::Ok );
            REQUIRE( doc["locked"] == true );
            REQUIRE( doc["invalid"] == 0 );
            JsonObject val = doc["val"];
            REQUIRE( STRCMP( val["type"], "eHP" ) );
            REQUIRE( val["numStages"] == 1 );
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    SECTION( "fromJSON" )
    {
        CPL_SYSTEM_TRACE_SCOPE( SECT_, "fromJSON test" );

        // Start with MP in the invalid state
        Cpl::Text::FString<MAX_STR_LENG> string;
        Cpl::Text::DString               errorMsg( "noerror", 1024 );
        mp_apple_.removeLock();
        mp_orange_.removeLock();
        mp_orange_.setInvalid();
        uint16_t seqNum = mp_apple_.setInvalid();
        uint16_t seqNum2;
        Cpl::Dm::ModelPoint* mp;

        SECTION( "Write value" )
        {
            const char* json = "{name:\"APPLE\", val:{type:\"eAC\", numStages:10}}";
            bool result = modelDb_.fromJSON( json, &errorMsg, &mp, &seqNum2 );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( seqNum2 == seqNum + 1 );
            MpOduConfig::Data value;
            int8_t valid;
            valid = mp_apple_.read( value, &seqNum );
            REQUIRE( seqNum == seqNum2 );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) );
            REQUIRE( value.numStages == OPTION_STORM_DM_ODU_CONFIG_MAX_COMPRESSOR_STAGES );
            REQUIRE( value.type == Storm::Type::OduType::eAC );
            REQUIRE( errorMsg == "noerror" );
            REQUIRE( mp == &mp_apple_ );
        }

        SECTION( "Write value - error cases" )
        {
            const char* json   = "{name:\"APPLE\", val:\"abc\"}";
            bool        result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == false );

            errorMsg = "noerror";
            json     = "{name:\"APPLE\"}";
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == false );
            REQUIRE( errorMsg != "noerror" );

            errorMsg = "noerror";
            json     = "{namex:\"APPLE\"}";
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == false );
            REQUIRE( errorMsg != "noerror" );

            errorMsg = "noerror";
            json     = "{name:\"APPLE\", val:a123}";
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == false );
            REQUIRE( errorMsg != "noerror" );

            errorMsg = "noerror";
            json     = "{name:\"APPLE\", val:{}}";
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == false );
            REQUIRE( errorMsg != "noerror" );

            errorMsg = "noerror";
            json     = "{name:\"APPLE\", val:{numHeat:a}}";
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg =[%s])", errorMsg.getString() ) );
            REQUIRE( result == false );
            REQUIRE( errorMsg != "noerror" );

            errorMsg = "noerror";
            json     = "{name:\"BOB\", invalid:1}";
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == false );
        }


        SECTION( "Set Invalid" )
        {
            MpOduConfig::Data value = { Storm::Type::OduType::eHP, 1 };
            uint16_t seqNum = mp_apple_.write( value );
            const char* json = "{name:\"APPLE\", val:{numStages:0}, invalid:1}";
            bool result = modelDb_.fromJSON( json, &errorMsg, &mp, &seqNum2 );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( seqNum2 == seqNum + 1 );
            int8_t valid;
            valid = mp_apple_.read( value, &seqNum );
            REQUIRE( seqNum == seqNum2 );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == false );
            REQUIRE( errorMsg == "noerror" );
            REQUIRE( mp == &mp_apple_ );
        }

        SECTION( "lock..." )
        {
            const char* json = "{name:\"APPLE\", val:{numStages:0}, locked:true}";
            bool result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == true );
            int8_t valid;
            MpOduConfig::Data value;
            valid = mp_apple_.read( value );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
            REQUIRE( errorMsg == "noerror" );
            REQUIRE( mp_apple_.isLocked() == true );
            REQUIRE( value.numStages == 0 );
            REQUIRE( value.type == OPTION_STORM_DM_ODU_CONFIG_DEFAULT_ODUTYPE );

            json   = "{name:\"APPLE\", invalid:21, locked:false}";
            result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( mp_apple_.isNotValid() == true );
            REQUIRE( mp_apple_.isLocked() == false );
            REQUIRE( mp_apple_.getValidState() == 21 );

            json   = "{name:\"APPLE\", val:{numStages:1}, locked:true}";
            result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( mp_apple_.isLocked() == true );
            valid = mp_apple_.read( value );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
            REQUIRE( value.numStages == 1 );
            REQUIRE( value.type == OPTION_STORM_DM_ODU_CONFIG_DEFAULT_ODUTYPE );

            json   = "{name:\"APPLE\", locked:false}";
            result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == true );
            valid = mp_apple_.read( value );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
            REQUIRE( value.numStages == 1 );
            REQUIRE( value.type == OPTION_STORM_DM_ODU_CONFIG_DEFAULT_ODUTYPE );
            REQUIRE( mp_apple_.isLocked() == false );
        }
    }

    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}

