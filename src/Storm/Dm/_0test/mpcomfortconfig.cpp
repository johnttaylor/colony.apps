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
#include "Storm/Dm/MpComfortConfig.h"
#include "common.h"
#include <string.h>



using namespace Storm::Dm;
static Cpl::Dm::MailboxServer     t1Mbox_;

#define STRCMP(a,b)               (strcmp(a,b) == 0)

/////////////////////////////////////////////////////////////////
namespace {
class Rmw : public MpComfortConfig::Client
{
public:
    ///
    int                                         m_callbackCount;
    ///
    Cpl::Dm::ModelPoint::RmwCallbackResult_T    m_returnResult;
    ///
    uint32_t                                    m_onTime;

public:
    ///
    Rmw() :m_callbackCount( 0 ), m_returnResult( Cpl::Dm::ModelPoint::eNO_CHANGE ), m_onTime( 0 ) {}
    ///
    Cpl::Dm::ModelPoint::RmwCallbackResult_T callback( MpComfortConfig::Data& data, int8_t validState ) noexcept
    {
        m_callbackCount++;
        if ( m_returnResult != Cpl::Dm::ModelPoint::eNO_CHANGE )
        {
            data.cooling[0].minOnTime= m_onTime;
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
static MpComfortConfig          mp_apple_( modelDb_, info_mp_apple_ );

static Cpl::Dm::StaticInfo      info_mp_orange_( "ORANGE" );
static MpComfortConfig          mp_orange_( modelDb_, info_mp_orange_ );


////////////////////////////////////////////////////////////////////////////////
TEST_CASE( "MP Comfort Config" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();

    SECTION( "ReadWrite" )
    {
        CPL_SYSTEM_TRACE_SCOPE( SECT_, "READWRITE test" );

        // Read
        MpComfortConfig::Data   value;
        uint16_t                seqNum;
        int8_t                  valid = mp_orange_.read( value, &seqNum );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.cooling[0].cph == Storm::Type::Cph::e3CPH );
        REQUIRE( value.cooling[0].minOnTime == ( 5 * 60 * 1000 ) );
        REQUIRE( value.cooling[0].minOffTime == ( 5 * 60 * 1000 ) );
        REQUIRE( value.heating[0].cph == Storm::Type::Cph::e3CPH );
        REQUIRE( value.heating[0].minOnTime == ( 5 * 60 * 1000 ) );
        REQUIRE( value.heating[0].minOffTime == ( 5 * 60 * 1000 ) );

        // Write
        value.cooling[0].cph        = Storm::Type::Cph::e6CPH;
        value.cooling[0].minOnTime  = 10;
        value.cooling[0].minOffTime = 20;
        value.heating[0].cph        = Storm::Type::Cph::e5CPH;
        value.heating[0].minOnTime  = 50;
        value.heating[0].minOffTime = 60;
        uint16_t seqNum2 = mp_apple_.write( value );
        valid = mp_apple_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.cooling[0].cph == Storm::Type::Cph::e6CPH );
        REQUIRE( value.cooling[0].minOnTime == 10 );
        REQUIRE( value.cooling[0].minOffTime == 20 );
        REQUIRE( value.heating[0].cph == Storm::Type::Cph::e5CPH );
        REQUIRE( value.heating[0].minOnTime == 50 );
        REQUIRE( value.heating[0].minOffTime == 60 );
        REQUIRE( seqNum + 1 == seqNum2 );

        // Write out-of-range
        value.cooling[0].cph        = Storm::Type::Cph::eNUM_OPTIONS;
        value.cooling[0].minOnTime  = 100 * 60 * 1000;
        value.cooling[0].minOffTime = 20;
        value.heating[0].cph        = -1;
        value.heating[0].minOnTime  = 50 * 60 * 1000;
        value.heating[0].minOffTime = 60;
        seqNum = mp_apple_.write( value );
        valid = mp_apple_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.cooling[0].cph == Storm::Type::Cph::e3CPH );
        REQUIRE( value.cooling[0].minOnTime == ( 10 * 60 * 1000 ) );
        REQUIRE( value.cooling[0].minOffTime == 20 );
        REQUIRE( value.heating[0].cph == Storm::Type::Cph::e3CPH );
        REQUIRE( value.heating[0].minOnTime == ( 10 * 60 * 1000 ) );
        REQUIRE( value.heating[0].minOffTime == 60 );
        REQUIRE( seqNum == seqNum2 + 1 );

        // Read-Modify-Write with Lock
        Rmw callbackClient;
        callbackClient.m_callbackCount  = 0;
        callbackClient.m_onTime         = 1;
        callbackClient.m_returnResult   = Cpl::Dm::ModelPoint::eCHANGED;
        mp_apple_.readModifyWrite( callbackClient, Cpl::Dm::ModelPoint::eLOCK );
        valid = mp_apple_.read( value );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        bool locked = mp_apple_.isLocked();
        REQUIRE( locked == true );
        REQUIRE( value.cooling[0].minOnTime == 1 );
        REQUIRE( value.cooling[0].minOffTime == 20 );
        REQUIRE( value.cooling[0].cph == Storm::Type::Cph::e3CPH );
        REQUIRE( callbackClient.m_callbackCount == 1 );

        // Read-Modify-Write with out-of-range values
        callbackClient.m_callbackCount  = 0;
        callbackClient.m_onTime         = 20 * 60 * 1000;
        callbackClient.m_returnResult   = Cpl::Dm::ModelPoint::eCHANGED;
        mp_apple_.readModifyWrite( callbackClient, Cpl::Dm::ModelPoint::eUNLOCK );
        valid  = mp_apple_.read( value );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.cooling[0].minOnTime == ( 10 * 60 * 1000 ) );
        REQUIRE( value.cooling[0].minOffTime == 20 );
        REQUIRE( value.cooling[0].cph == Storm::Type::Cph::e3CPH );
        REQUIRE( callbackClient.m_callbackCount == 1 );

        // Invalidate with Unlock
        mp_apple_.setInvalidState( 112, Cpl::Dm::ModelPoint::eUNLOCK );
        REQUIRE( mp_apple_.isNotValid() == true );
        valid = mp_apple_.getValidState();
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == false );
        REQUIRE( valid == 112 );

        // Single writes
        MpComfortConfig::Parameters_T parms = { Storm::Type::Cph::e4CPH, 111, 222 };
        mp_apple_.writeCoolingStage( 0, parms );
        valid = mp_apple_.read( value );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.cooling[0].minOnTime == 111 );
        REQUIRE( value.cooling[0].minOffTime == 222 );
        REQUIRE( value.cooling[0].cph == Storm::Type::Cph::e4CPH );

        // Single writes
        parms = { Storm::Type::Cph::e3CPH, 333, 444 };
        mp_apple_.writeHeatingStage( 0, parms );
        valid = mp_apple_.read( value );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.heating[0].minOnTime == 333 );
        REQUIRE( value.heating[0].minOffTime == 444 );
        REQUIRE( value.heating[0].cph == Storm::Type::Cph::e3CPH );

        // Single writes (out-of-range)
        parms = { 44, 555, 666 };
        mp_apple_.writeCoolingStage( 0, parms );
        valid = mp_apple_.read( value );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.cooling[0].minOnTime == 555 );
        REQUIRE( value.cooling[0].minOffTime == 666 );
        REQUIRE( value.cooling[0].cph == Storm::Type::Cph::e3CPH );
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
        REQUIRE( s == sizeof( MpComfortConfig::Data ) );
        s = mp_orange_.getSize();
        REQUIRE( s == sizeof( MpComfortConfig::Data ) );

        s = mp_apple_.getExternalSize();
        REQUIRE( s == sizeof( MpComfortConfig::Data ) + sizeof( int8_t ) );
        s = mp_orange_.getExternalSize();
        REQUIRE( s == sizeof( MpComfortConfig::Data ) + sizeof( int8_t ) );

        const char* mpType = mp_apple_.getTypeAsText();
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "typeText: [%s])", mpType ) );
        REQUIRE( strcmp( mpType, "Storm::Dm::MpComfortConfig" ) == 0 );
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
        MpComfortConfig::Data value;
        memset( &value, 0, sizeof( value ) );
        value.cooling[0].minOnTime = 11;
        seqNum = mp_apple_.write( value );
        REQUIRE( seqNum == seqNum2 + 1 );
        int8_t valid;
        valid = mp_apple_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( mp_apple_.isNotValid() == false );
        REQUIRE( value.cooling[0].minOnTime == 11 );
        REQUIRE( value.cooling[0].minOffTime == 0 );
        REQUIRE( value.cooling[0].cph == 0 );
        REQUIRE( value.heating[0].minOnTime == 0 );
        REQUIRE( value.heating[0].minOffTime == 0 );
        REQUIRE( value.heating[0].cph == 0 );

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
        value.cooling[0].minOnTime = 22;
        value.heating[0].minOnTime = 11;
        seqNum = mp_apple_.write( value );
        REQUIRE( seqNum == seqNum2 + 1 );
        valid = mp_apple_.read( value );
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
        REQUIRE( value.cooling[0].minOnTime == 22 );
        REQUIRE( value.cooling[0].minOffTime == 0 );
        REQUIRE( value.cooling[0].cph == 0 );
        REQUIRE( value.heating[0].minOnTime == 11 );
        REQUIRE( value.heating[0].minOffTime == 0 );
        REQUIRE( value.heating[0].cph == 0 );

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
        REQUIRE( value.cooling[0].minOnTime == 22 );
        REQUIRE( value.cooling[0].minOffTime == 0 );
        REQUIRE( value.cooling[0].cph == 0 );
        REQUIRE( value.heating[0].minOnTime == 11 );
        REQUIRE( value.heating[0].minOffTime == 0 );
        REQUIRE( value.heating[0].cph == 0 );
    }

    ///////////////////////////////////////////////////////////////////////////////


    SECTION( "observer" )
    {
        CPL_SYSTEM_TRACE_SCOPE( SECT_, "observer TEST" );

        Cpl::System::Thread* t1 = Cpl::System::Thread::create( t1Mbox_, "T1" );
        AsyncClient<MpComfortConfig> viewer1( t1Mbox_, Cpl::System::Thread::getCurrent(), mp_apple_ );

        // Open, write a value, wait for Viewer to see the change, then close
        mp_apple_.removeLock();
        viewer1.open();
        MpComfortConfig::Data value;
        memset( &value, 0, sizeof( value ) );
        value.cooling[0].minOnTime = 11;
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
            MpComfortConfig::Data value;
            value.cooling[0].cph        = 0;
            value.cooling[0].minOnTime  = 111;
            value.cooling[0].minOffTime = 222;
            value.heating[0].cph        = 1;
            value.heating[0].minOnTime  = 333;
            value.heating[0].minOffTime = 444;
            uint16_t seqnum  = mp_apple_.write( value, Cpl::Dm::ModelPoint::eUNLOCK );
            mp_apple_.toJSON( string, MAX_STR_LENG, truncated );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "toJSON: (%s)", string ) );

            StaticJsonDocument<1024> doc;
            DeserializationError err = deserializeJson( doc, string );
            REQUIRE( err == DeserializationError::Ok );
            REQUIRE( doc["seqnum"] == seqnum );
            REQUIRE( doc["locked"] == false );
            REQUIRE( doc["invalid"] == 0 );
            JsonObject val = doc["val"];
            REQUIRE( STRCMP( val["cool"][0]["cph"], "e2CPH" ) );
            REQUIRE( val["cool"][0]["minOn"] == 111 );
            REQUIRE( val["cool"][0]["minOff"] == 222 );
            REQUIRE( STRCMP( val["heat"][0]["cph"], "e3CPH" ) );
            REQUIRE( val["heat"][0]["minOn"] == 333 );
            REQUIRE( val["heat"][0]["minOff"] == 444 );
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
            REQUIRE( STRCMP( val["cool"][0]["cph"], "e2CPH" ) );
            REQUIRE( val["cool"][0]["minOn"] == 111 );
            REQUIRE( val["cool"][0]["minOff"] == 222 );
            REQUIRE( STRCMP( val["heat"][0]["cph"], "e3CPH" ) );
            REQUIRE( val["heat"][0]["minOn"] == 333 );
            REQUIRE( val["heat"][0]["minOff"] == 444 );
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
            const char* json = "{\"name\":\"APPLE\",\"val\":{\"cool\":[{\"stage\":1,\"cph\":\"e2CPH\",\"minOn\":111,\"minOff\":222}],\"heat\":[{\"stage\":1,\"cph\":\"e3CPH\",\"minOn\":333,\"minOff\":444}]}}";
            bool result = modelDb_.fromJSON( json, &errorMsg, &mp, &seqNum2 );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( seqNum2 == seqNum + 1 );
            MpComfortConfig::Data value;
            int8_t valid;
            valid = mp_apple_.read( value, &seqNum );
            REQUIRE( seqNum == seqNum2 );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) );
            REQUIRE( value.cooling[0].cph == Storm::Type::Cph::e2CPH );
            REQUIRE( value.cooling[0].minOnTime == 111 );
            REQUIRE( value.cooling[0].minOffTime == 222 );
            REQUIRE( value.heating[0].cph == Storm::Type::Cph::e3CPH );
            REQUIRE( value.heating[0].minOnTime == 333 );
            REQUIRE( value.heating[0].minOffTime == 444 );
            REQUIRE( errorMsg == "noerror" );
            REQUIRE( mp == &mp_apple_ );

            json = "{\"name\":\"APPLE\",\"val\":{\"cool\":[{\"stage\":1,\"minOff\":666}],\"heat\":[{\"stage\":1,\"minOn\":777}]}}";
            result = modelDb_.fromJSON( json, &errorMsg, &mp, &seqNum2 );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( seqNum2 == seqNum + 1 );
            valid = mp_apple_.read( value, &seqNum );
            REQUIRE( seqNum == seqNum2 );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) );
            REQUIRE( value.cooling[0].cph == Storm::Type::Cph::e2CPH );
            REQUIRE( value.cooling[0].minOnTime == 111 );
            REQUIRE( value.cooling[0].minOffTime == 666 );
            REQUIRE( value.heating[0].cph == Storm::Type::Cph::e3CPH );
            REQUIRE( value.heating[0].minOnTime == 777 );
            REQUIRE( value.heating[0].minOffTime == 444 );
            REQUIRE( errorMsg == "noerror" );
            REQUIRE( mp == &mp_apple_ );
        }

        SECTION( "Write value - error cases" )
        {
            MpComfortConfig::Parameters_T parms = { Storm::Type::Cph::e6CPH, 5, 5 };
            seqNum = mp_apple_.writeCoolingStage( 0, parms );
            const char* json   = "{name:\"APPLE\", val:\"abc\"}";
            bool        result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( seqNum == mp_apple_.getSequenceNumber() ); // NOT a failure -->but nothing should have been updated/changed!

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

            seqNum   = mp_apple_.getSequenceNumber();
            errorMsg = "noerror";
            json     = "{name:\"APPLE\", val:{}}";
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( seqNum == mp_apple_.getSequenceNumber() ); // NOT a failure -->but nothing should have been updated/changed!

            errorMsg = "noerror";
            json     = "{name:\"APPLE\", val:{numHeat:a}}";
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg =[%s])", errorMsg.getString() ) );
            REQUIRE( result == false );

            errorMsg = "noerror";
            json     = "{name:\"BOB\", invalid:1}";
            result   = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == false );
        }


        SECTION( "Set Invalid" )
        {
            MpComfortConfig::Data value;
            memset( &value, 0, sizeof( value ) );
            value.cooling[0].minOnTime = 11;
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
            const char* json = "{name:\"APPLE\", \"val\":{\"cool\":[{\"stage\":1,\"cph\":\"e3CPH\",\"minOn\":211,\"minOff\":322}],\"heat\":[{\"stage\":1,\"cph\":\"e4CPH\",\"minOn\":433,\"minOff\":544}]}, locked:true}";
            bool result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == true );
            int8_t valid;
            MpComfortConfig::Data value;
            valid = mp_apple_.read( value );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
            REQUIRE( errorMsg == "noerror" );
            REQUIRE( mp_apple_.isLocked() == true );
            REQUIRE( value.cooling[0].cph == Storm::Type::Cph::e3CPH );
            REQUIRE( value.cooling[0].minOnTime == 211 );
            REQUIRE( value.cooling[0].minOffTime == 322 );
            REQUIRE( value.heating[0].cph == Storm::Type::Cph::e4CPH );
            REQUIRE( value.heating[0].minOnTime == 433 );
            REQUIRE( value.heating[0].minOffTime == 544 );

            json   = "{name:\"APPLE\", invalid:21, locked:false}";
            result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( mp_apple_.isNotValid() == true );
            REQUIRE( mp_apple_.isLocked() == false );
            REQUIRE( mp_apple_.getValidState() == 21 );

            json   = "{name:\"APPLE\", \"val\":{\"cool\":[{\"stage\":1,\"cph\":\"e6CPH\"}]}, locked:true}";
            result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == true );
            REQUIRE( mp_apple_.isLocked() == true );
            valid = mp_apple_.read( value );
            REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( valid ) == true );
            REQUIRE( value.cooling[0].cph == Storm::Type::Cph::e6CPH );
            REQUIRE( value.cooling[0].minOnTime == ( 5 * 60 * 1000 ) );
            REQUIRE( value.cooling[0].minOffTime == ( 5 * 60 * 1000 ) );
            REQUIRE( value.heating[0].cph == Storm::Type::Cph::e3CPH );
            REQUIRE( value.heating[0].minOnTime == ( 5 * 60 * 1000 ) );
            REQUIRE( value.heating[0].minOffTime == ( 5 * 60 * 1000 ) );

            json   = "{name:\"APPLE\", locked:false}";
            result = modelDb_.fromJSON( json, &errorMsg );
            CPL_SYSTEM_TRACE_MSG( SECT_, ( "fromSJON errorMsg=[%s])", errorMsg.getString() ) );
            REQUIRE( result == true );
            valid = mp_apple_.read( value );
            REQUIRE( value.cooling[0].cph == Storm::Type::Cph::e6CPH );
            REQUIRE( value.cooling[0].minOnTime == ( 5 * 60 * 1000 ) );
            REQUIRE( value.cooling[0].minOffTime == ( 5 * 60 * 1000 ) );
            REQUIRE( value.heating[0].cph == Storm::Type::Cph::e3CPH );
            REQUIRE( value.heating[0].minOnTime == ( 5 * 60 * 1000 ) );
            REQUIRE( value.heating[0].minOffTime == ( 5 * 60 * 1000 ) );
            REQUIRE( mp_apple_.isLocked() == false );
        }
    }

    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}
