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

#include "Catch/catch.hpp"
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Api.h"
#include "Cpl/Text/FString.h"
#include "Cpl/Text/DString.h"
#include "Cpl/Dm/ModelDatabase.h"
#include "Storm/Dm/MpHvacRelayOutputs.h"
#include "common.h"
#include <string.h>



using namespace Storm::Dm;
static Cpl::Dm::MailboxServer     t1Mbox_;

#define STRCMP(a,b)               (strcmp(a,b) == 0)

/////////////////////////////////////////////////////////////////
namespace {
class Rmw : public MpHvacRelayOutputs::Client
{
public:
    ///
    int                                         m_callbackCount;
    ///
    Cpl::Dm::ModelPoint::RmwCallbackResult_T    m_returnResult;
    ///
    bool                                        m_gOut;

public:
    ///
    Rmw() :m_callbackCount( 0 ), m_returnResult( Cpl::Dm::ModelPoint::eNO_CHANGE ), m_gOut( 0 ) {}
    ///
    Cpl::Dm::ModelPoint::RmwCallbackResult_T callback( Storm::Type::HvacRelayOutputs_T& data, int8_t validState ) noexcept
    {
        m_callbackCount++;
        if ( m_returnResult != Cpl::Dm::ModelPoint::eNO_CHANGE )
        {
            data.g = m_gOut;
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
static MpHvacRelayOutputs       mp_apple_( modelDb_, info_mp_apple_ );

static Cpl::Dm::StaticInfo      info_mp_orange_( "ORANGE" );
static MpHvacRelayOutputs       mp_orange_( modelDb_, info_mp_orange_ );

////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "MP HVAC Relay Outputs" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();

    SECTION( "ReadWrite" )
    {
        CPL_SYSTEM_TRACE_SCOPE( SECT_, "READWRITE test" );

        // Read
        Storm::Type::HvacRelayOutputs_T writeValue;
        Storm::Dm::MpHvacRelayOutputs::setSafeAllOff( writeValue );
        writeValue.o = false;

        Storm::Type::HvacRelayOutputs_T    value;
        uint16_t            seqNum;
        int8_t              valid = mp_orange_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        valid = mp_apple_.read( value, &seqNum );
        REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );

        // Write
        writeValue.o  = true;
        writeValue.w1 = true;
        uint16_t seqNum2 = mp_apple_.write( writeValue );
        valid = mp_apple_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );
        REQUIRE( seqNum + 1 == seqNum2 );

        // Write
        writeValue.y1 = true;
        seqNum = mp_apple_.write( writeValue );
        valid = mp_apple_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );
        REQUIRE( seqNum == seqNum2 + 1 );
        REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );

        // Write
        writeValue.y2 = true;
        seqNum2 = mp_apple_.write( writeValue );
        valid = mp_apple_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( seqNum + 1 == seqNum2 );
        REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );

        // Read-Modify-Write with Lock
        Rmw callbackClient;
        callbackClient.m_callbackCount  = 0;
        callbackClient.m_gOut           = true;
        callbackClient.m_returnResult   = Cpl::Dm::ModelPoint::eCHANGED;
        mp_apple_.readModifyWrite( callbackClient, Cpl::Dm::ModelPoint::eLOCK );
        valid = mp_apple_.read( value );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        bool locked = mp_apple_.isLocked();
        REQUIRE( locked == true );
        writeValue.g = callbackClient.m_gOut;
        REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );
        REQUIRE( callbackClient.m_callbackCount == 1 );

        // Write with Unlock
        Storm::Dm::MpHvacRelayOutputs::setSafeAllOff( writeValue );
        mp_apple_.setSafeAllOff( Cpl::Dm::ModelPoint::eUNLOCK );
        valid = mp_apple_.read( value );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );

        // Invalidate with Unlock
        mp_apple_.setInvalidState( 112, Cpl::Dm::ModelPoint::eUNLOCK );
        REQUIRE( mp_apple_.isNotValid() == true );
        valid = mp_apple_.getValidState();
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == false );
        REQUIRE( valid == 112 );

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
        REQUIRE( s == sizeof( Storm::Type::HvacRelayOutputs_T ) );
        s = mp_orange_.getSize();
        REQUIRE( s == sizeof( Storm::Type::HvacRelayOutputs_T ) );

        s = mp_apple_.getExternalSize();
        REQUIRE( s == sizeof( Storm::Type::HvacRelayOutputs_T ) + sizeof( int8_t ) );
        s = mp_orange_.getExternalSize();
        REQUIRE( s == sizeof( Storm::Type::HvacRelayOutputs_T ) + sizeof( int8_t ) );

        const char* mpType = mp_apple_.getTypeAsText();
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "typeText: (%s)", mpType ) );
        REQUIRE( strcmp( mpType, "Storm::Dm::MpHvacRelayOutputs" ) == 0 );
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
        Storm::Type::HvacRelayOutputs_T writeValue;
        Storm::Dm::MpHvacRelayOutputs::setSafeAllOff( writeValue );
        writeValue.o = true;
        seqNum = mp_apple_.write( writeValue );
        REQUIRE( seqNum == seqNum2 + 1 );
        Storm::Type::HvacRelayOutputs_T value;
        int8_t           valid;
        valid = mp_apple_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );

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
        writeValue.y1 = true;
        seqNum = mp_apple_.write( writeValue );
        REQUIRE( seqNum == seqNum2 + 1 );
        valid = mp_apple_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );

        // Export...
        REQUIRE( mp_apple_.isNotValid() == false );
        b = mp_apple_.exportData( streamBuffer, sizeof( streamBuffer ), &seqNum2 );
        REQUIRE( b != 0 );
        REQUIRE( b == mp_apple_.getExternalSize() );
        REQUIRE( seqNum == seqNum2 );

        // Set and new value AND invalidate the MP
        writeValue.y2 = true;
        seqNum = mp_apple_.write( writeValue );
        seqNum = mp_apple_.setInvalid();
        REQUIRE( seqNum == seqNum2 + 2 );
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
        writeValue.y2 = false;
        REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );
    }

    ///////////////////////////////////////////////////////////////////////////////


    SECTION( "observer" )
    {
        CPL_SYSTEM_TRACE_SCOPE( SECT_, "observer TEST" );

        mp_apple_.setInvalid();
        Cpl::System::Thread* t1 = Cpl::System::Thread::create( t1Mbox_, "T1" );
        AsyncClient<MpHvacRelayOutputs> viewer1( t1Mbox_, Cpl::System::Thread::getCurrent(), mp_apple_ );

        // Open, write a value, wait for Viewer to see the change, then close
        mp_apple_.removeLock();
        viewer1.open();
        Storm::Type::HvacRelayOutputs_T writeValue;
        Storm::Dm::MpHvacRelayOutputs::setSafeAllOff( writeValue );
        writeValue.o = true;
        uint16_t seqNum = mp_apple_.write( writeValue );
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
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: terse (%s)", string ) );
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
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: (%s)", string ) );
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
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: (%s)", string ) );

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
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: (%s)", string ) );

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
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: (%s)", string ) );

            StaticJsonDocument<1024> doc;
            DeserializationError err = deserializeJson( doc, string );
            REQUIRE( err == DeserializationError::Ok );
            REQUIRE( doc["invalid"] == 100 );
            REQUIRE( doc["locked"] == true );
        }

        SECTION( "Value" )
        {
            Storm::Type::HvacRelayOutputs_T writeValue;
            Storm::Dm::MpHvacRelayOutputs::setSafeAllOff( writeValue );
            writeValue.o  = false;
            writeValue.g  = true;
            writeValue.bk = 11;
            writeValue.y2 = true;
            writeValue.w1 = true;
            writeValue.w3 = true;
            uint16_t seqnum = mp_apple_.write( writeValue, Cpl::Dm::ModelPoint::eUNLOCK );
            mp_apple_.toJSON( string, MAX_STR_LENG, truncated );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: (%s)", string ) );

            StaticJsonDocument<1024> doc;
            DeserializationError err = deserializeJson( doc, string );
            REQUIRE( err == DeserializationError::Ok );
            REQUIRE( doc["seqnum"] == seqnum );
            REQUIRE( doc["locked"] == false );
            REQUIRE( doc["invalid"] == 0 );
            JsonObject val = doc["val"];
            REQUIRE( val["g"] == true );
            REQUIRE( val["bk"] == 11 );
            REQUIRE( STRCMP( val["o"], "HEAT" ) );
            REQUIRE( val["y1"] == false );
            REQUIRE( val["y2"] == true );
            REQUIRE( val["w1"] == true );
            REQUIRE( val["w2"] == false );
            REQUIRE( val["w3"] == true );
        }

        SECTION( "Value + Lock" )
        {
            mp_apple_.applyLock();
            mp_apple_.toJSON( string, MAX_STR_LENG, truncated );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: (%s)", string ) );

            StaticJsonDocument<1024> doc;
            DeserializationError err = deserializeJson( doc, string );
            REQUIRE( err == DeserializationError::Ok );
            REQUIRE( doc["locked"] == true );
            REQUIRE( doc["invalid"] == 0 );
            JsonObject val = doc["val"];
            REQUIRE( val["g"] == true );
            REQUIRE( val["bk"] == 11 );
            REQUIRE( STRCMP( val["o"], "HEAT" ) );
            REQUIRE( val["y1"] == false );
            REQUIRE( val["y2"] == true );
            REQUIRE( val["w1"] == true );
            REQUIRE( val["w2"] == false );
            REQUIRE( val["w3"] == true );
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
            const char* json = "{\"name\":\"APPLE\",\"val\":{g:true,bk:12,w1:false,w2:true,w3:false,y1:true,y2:false,o:\"COOL\"}}";
            bool result = modelDb_.fromJSON( json, &errorMsg, &mp, &seqNum2 );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=(%s)", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( seqNum2 == seqNum + 1 );
            Storm::Type::HvacRelayOutputs_T value;
            int8_t           valid = mp_apple_.read( value, &seqNum );
            REQUIRE( seqNum == seqNum2 );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) );
            Storm::Type::HvacRelayOutputs_T writeValue;
            Storm::Dm::MpHvacRelayOutputs::setSafeAllOff( writeValue );
            writeValue.g  = true;
            writeValue.bk = 12;
            writeValue.w2 = true;
            writeValue.y1 = true;
            writeValue.o  = true;
            REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );
            REQUIRE( errorMsg == "noerror" );
            REQUIRE( mp == &mp_apple_ );
        }

        SECTION( "Write value - error cases" )
        {
            Storm::Type::HvacRelayOutputs_T writeValue;
            Storm::Dm::MpHvacRelayOutputs::setSafeAllOff( writeValue );
            writeValue.o = false;
            uint16_t seqNum = mp_apple_.write( writeValue );
            const char* json   = "{name:\"APPLE\", val:\"abc\"}";
            bool        result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=(%s)", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( seqNum == mp_apple_.getSequenceNumber() ); // JSON parsing 'passed' -->but NO CHANGE to the actual MP.

            errorMsg = "noerror";
            json     = "{name:\"APPLE\"}";
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=(%s)", errorMsg.getString() ) );
            REQUIRE( result == false );
            REQUIRE( errorMsg != "noerror" );

            errorMsg = "noerror";
            json     = "{namex:\"APPLE\"}";
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=(%s)", errorMsg.getString() ) );
            REQUIRE( result == false );
            REQUIRE( errorMsg != "noerror" );

            errorMsg = "noerror";
            json     = "{name:\"APPLE\", val:a123}";
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=(%s)", errorMsg.getString() ) );
            REQUIRE( result == false );
            REQUIRE( errorMsg != "noerror" );

            errorMsg = "noerror";
            json     = "{name:\"APPLE\", val:{}}";
            seqNum   = mp_apple_.getSequenceNumber();
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=(%s)", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( seqNum == mp_apple_.getSequenceNumber() ); // JSON parsing 'passed' -->but NO CHANGE to the actual MP.

            errorMsg = "noerror";
            json     = "{name:\"APPLE\", val:{priAlarm:123}}";
            seqNum   = mp_apple_.getSequenceNumber();
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg =(%s)", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( seqNum == mp_apple_.getSequenceNumber() ); // JSON parsing 'passed' -->but NO CHANGE to the actual MP.

            errorMsg = "noerror";
            json     = "{name:\"BOB\", invalid:1}";
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=(%s)", errorMsg.getString() ) );
            REQUIRE( result == false );
        }



        SECTION( "Set Invalid" )
        {
            Storm::Type::HvacRelayOutputs_T writeValue;
            Storm::Dm::MpHvacRelayOutputs::setSafeAllOff( writeValue );
            writeValue.o = true;
            writeValue.g = true;
            uint16_t seqNum = mp_apple_.write( writeValue );
            const char* json = "{name:\"APPLE\", invalid:1, \"val\":{g:true,bk:12,w1:false,w2:true,w3:false,y1:true,y2:false,o:\"COOL\"}}";
            bool result = modelDb_.fromJSON( json, &errorMsg, &mp, &seqNum2 );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=(%s)", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( seqNum2 == seqNum + 1 );
            Storm::Type::HvacRelayOutputs_T value;
            int8_t valid = mp_apple_.read( value, &seqNum );
            REQUIRE( seqNum == seqNum2 );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == false );
            REQUIRE( errorMsg == "noerror" );
            REQUIRE( mp == &mp_apple_ );
        }

        SECTION( "lock..." )
        {
            const char* json = "{name:\"APPLE\", \"val\":{g:true,bk:12,w1:false,w2:true,w3:false,y1:true,y2:false,o:\"COOL\"}, locked:true}";
            bool result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=(%s)", errorMsg.getString() ) );
            REQUIRE( result == true );
            Storm::Type::HvacRelayOutputs_T value;
            int8_t           valid = mp_apple_.read( value );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
            REQUIRE( errorMsg == "noerror" );
            REQUIRE( mp_apple_.isLocked() == true );
            Storm::Type::HvacRelayOutputs_T writeValue;
            Storm::Dm::MpHvacRelayOutputs::setSafeAllOff( writeValue );
            writeValue.g  = true;
            writeValue.bk = 12;
            writeValue.w2 = true;
            writeValue.y1 = true;
            writeValue.o  = true;
            REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );

            json   = "{name:\"APPLE\", invalid:21, locked:false}";
            result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=(%s)", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( mp_apple_.isNotValid() == true );
            REQUIRE( mp_apple_.isLocked() == false );
            REQUIRE( mp_apple_.getValidState() == 21 );
            Storm::Dm::MpHvacRelayOutputs::setSafeAllOff( writeValue );
            writeValue.o = false;

            json   = "{name:\"APPLE\", \"val\":{w2:true}, locked:true}";
            result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=(%s)", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( mp_apple_.isLocked() == true );
            valid = mp_apple_.read( value );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
            writeValue.w2  = true;
            REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );

            json   = "{name:\"APPLE\", \"val\":{g:false,y1:false} }";
            result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=(%s)", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( mp_apple_.isLocked() == true );
            valid = mp_apple_.read( value );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
            REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );

            json   = "{name:\"APPLE\", locked:false}";
            result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=(%s)", errorMsg.getString() ) );
            REQUIRE( result == true );
            valid = mp_apple_.read( value );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
            REQUIRE( memcmp( &writeValue, &value, sizeof( writeValue ) ) == 0 );
            REQUIRE( mp_apple_.isLocked() == false );
        }
    }

    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}

