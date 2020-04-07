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
#include "Storm/Thermostat/Persistence/UserRecord.h"
#include "Storm/Thermostat/ModelPoints.h"
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Trace.h"
#include "Cpl/System/Thread.h"
#include "Cpl/System/Api.h"
#include "Cpl/Text/FString.h"
#include "Cpl/Text/DString.h"
#include "Cpl/Checksum/Crc32EthernetFast.h"
#include "Cpl/Dm/ModelDatabase.h"
#include "Cpl/Dm/Persistence/Framing.h"
#include "Cpl/Dm/Persistence/Record/Server.h"
#include "Cpl/Dm/Persistence/Record/ServerApi.h"
#include "Cpl/Dm/Persistence/Chunk/FileSystem.h"
#include "Cpl/Dm/Persistence/Chunk/Server.h"
#include "Cpl/Io/File/Api.h"
#include "Cpl/Io/File/Output.h"
#include <string.h>
#include <stdint.h>

using namespace Storm::Thermostat::Persistence;

#define MODEL_POINT_EXPORTED_OVERHEAD    6

#define MAX_NAME_FIELD_LEN      16
#define MIN_RECORD_SIZE         (Cpl::Dm::Persistence::eNLEN_SIZE + MAX_NAME_FIELD_LEN + MODEL_POINT_EXPORTED_OVERHEAD)
#define CHUNK_OVERHEAD          (Cpl::Dm::Persistence::eCLEN_SIZE + Cpl::Dm::Persistence::eCCRC_SIZE)
#define RECORD_OVERHEAD         (Cpl::Dm::Persistence::eNLEN_SIZE + OPTION_CPL_DM_PERSISTENCE_RECORD_MAX_RECORD_NAME_LEN)

#define USER_RECORD_SIZE        (MIN_RECORD_SIZE + 8 + 4 + 4 + 4)
#define INSTALLER_RECORD_SIZE   (MIN_RECORD_SIZE + 64)
#define RUNTIME_RECORD_SIZE     (MIN_RECORD_SIZE + 64)
#define MAX_RECORD_SIZE         INSTALLER_RECORD_SIZE

// Relationship is 1 Record Per Chunk
#define MAX_CHUNK_BUF_SIZE      (MAX_RECORD_SIZE + RECORD_OVERHEAD + CHUNK_OVERHEAD)

#define MEDIA_FNAME_            "thermostatNVRAM"
#define SIGNATURE_              "thermostat-v1"


////////////////////////////////////////////////////////////////////////////////
static uint8_t                                                  recordLayerbuffer_[MAX_CHUNK_BUF_SIZE];
static Cpl::Container::Map<Cpl::Dm::Persistence::Record::Api_>  recordList_;

static Cpl::Checksum::Crc32EthernetFast                         chunkCrc_;
static Cpl::Dm::Persistence::Chunk::FileSystem                  mediaFile( MEDIA_FNAME_ );
static Cpl::Dm::Persistence::Chunk::Server                      chunkServer_( mediaFile, chunkCrc_, SIGNATURE_ );
static Cpl::Dm::MailboxServer                                   chunkMailbox_;
static Cpl::Dm::Persistence::Chunk::Request::SAP                chunkSap_( chunkServer_, chunkMailbox_ );
static Cpl::Dm::MailboxServer                                   recordServerMailbox_;
static Cpl::Dm::Persistence::Record::Server                     recordServer_( recordLayerbuffer_,
                                                                               sizeof( recordLayerbuffer_ ),
                                                                               chunkSap_,
                                                                               recordServerMailbox_,
                                                                               recordList_,
                                                                               mp_recordServerStatus,
                                                                               mp_recordDefaultActionRequest );

static Cpl::System::Thread* t1_;
static Cpl::System::Thread* t2_;

TEST_CASE( "records" )
{
    CPL_SYSTEM_TRACE_SCOPE( SECT_, "RECORDS TEST" );
    Cpl::System::Shutdown_TS::clearAndUseCounter();

    SECTION( "Initial load" )
    {
        // Ensure starting conditions
        Cpl::Io::File::Api::remove( MEDIA_FNAME_ );

        t1_ = Cpl::System::Thread::create( chunkMailbox_, "Chunk" );
        t2_ = Cpl::System::Thread::create( recordServerMailbox_, "Record" );
        REQUIRE( t1_ );
        REQUIRE( t2_ );

        // Load the records
        recordServer_.open();
        Cpl::System::Api::sleep( 500 );
        Cpl::Dm::Persistence::Record::ServerStatus serverStatus = Cpl::Dm::Persistence::Record::ServerStatus::eUNKNOWN;
        REQUIRE( Cpl::Dm::ModelPoint::IS_VALID( mp_recordServerStatus.read( serverStatus ) ) );
        REQUIRE( serverStatus == +Cpl::Dm::Persistence::Record::ServerStatus::eRUNNING );

        recordServer_.close();
        Cpl::System::Thread::destroy( *t1_ );
        Cpl::System::Thread::destroy( *t2_ );
    }
}

