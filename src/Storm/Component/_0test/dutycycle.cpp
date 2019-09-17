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
#include "Storm/Component/DutyCycle.h"
#include "Cpl/System/_testsupport/Shutdown_TS.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Math/real.h"
#include "statics.h"


using namespace Storm::Component;

#define SECT_       "_0test"


////////////////////////////////////////////////////////////////////////////////
void test_cph( DutyCycle::Cph_T cph )
{
    CPL_SYSTEM_TRACE_MSG( SECT_, ( "CPH TEST: CPH=%d", cph + 2 ) );
    
    float    pv      = 0.0F;
    uint32_t ontime  = DutyCycle::calculateOnTime( pv, 0, cph );
    uint32_t offtime = DutyCycle::calculateOffTime( pv, 0, cph );
    double   dutyCycle = ( ( ( double) ontime ) / ( ( double) ( ontime + offtime ) ) ) * 100.0F;
    CPL_SYSTEM_TRACE_MSG( SECT_, ( "pv=%g, ontime=%d, offtime=%d (dutyCycle=%g)", pv, ontime, offtime, dutyCycle ) );
    REQUIRE( ( ( int) pv ) == ( ( int) dutyCycle ) );
    REQUIRE( ontime < offtime );

    pv      = 33.0F;
    ontime  = DutyCycle::calculateOnTime( pv, 0, cph );
    offtime = DutyCycle::calculateOffTime( pv, 0, cph );
    dutyCycle = ( ( ( double) ontime ) / ( ontime + offtime ) ) * 100.0F;
    CPL_SYSTEM_TRACE_MSG( SECT_, ( "pv=%g, ontime=%d, offtime=%d (dutyCycle=%g)", pv, ontime, offtime, dutyCycle ) );
    REQUIRE( ( ( int) pv ) == ( ( int) dutyCycle ) );
    REQUIRE( ontime < offtime );

    pv      = 50.0F;
    ontime  = DutyCycle::calculateOnTime( pv, 0, cph );
    offtime = DutyCycle::calculateOffTime( pv, 0, cph );
    dutyCycle = ( ( ( double) ontime ) / ( ontime + offtime ) ) * 100.0F;
    CPL_SYSTEM_TRACE_MSG( SECT_, ( "pv=%g, ontime=%d, offtime=%d (dutyCycle=%g)", pv, ontime, offtime, dutyCycle ) );
    REQUIRE( ( ( int) pv ) == ( ( int) dutyCycle ) );
    REQUIRE( ontime == offtime );

    pv      = 75.0F;
    ontime  = DutyCycle::calculateOnTime( pv, 0, cph );
    offtime = DutyCycle::calculateOffTime( pv, 0, cph );
    dutyCycle = ( ( ( double) ontime ) / ( ontime + offtime ) ) * 100.0F;
    CPL_SYSTEM_TRACE_MSG( SECT_, ( "pv=%g, ontime=%d, offtime=%d (dutyCycle=%g)", pv, ontime, offtime, dutyCycle ) );
    REQUIRE( ( ( int) pv ) == ( ( int) dutyCycle ) );
    REQUIRE( ontime > offtime );

    pv      = 100.0F;
    ontime  = DutyCycle::calculateOnTime( pv, 0, cph );
    offtime = DutyCycle::calculateOffTime( pv, 0, cph );
    dutyCycle = ( ( ( double) ontime ) / ( ontime + offtime ) ) * 100.0F;
    CPL_SYSTEM_TRACE_MSG( SECT_, ( "pv=%g, ontime=%d, offtime=%d (dutyCycle=%g)", pv, ontime, offtime, dutyCycle ) );
    REQUIRE( ( ( int) pv ) == ( ( int) dutyCycle ) );
    REQUIRE( ontime > offtime );
}

TEST_CASE( "DUTY CYCLE" )
{
    Cpl::System::Shutdown_TS::clearAndUseCounter();

    SECTION( "CPHs" )
    {
        test_cph( DutyCycle::e2CPH );
        test_cph( DutyCycle::e3CPH );
        test_cph( DutyCycle::e4CPH );
        test_cph( DutyCycle::e5CPH );
        test_cph( DutyCycle::e6CPH );
    }

    SECTION( "Min times" )
    {
        float    pv      = 1.0F;
        uint32_t ontime  = DutyCycle::calculateOnTime( pv, 100000, DutyCycle::e6CPH );
        uint32_t offtime = DutyCycle::calculateOffTime( pv, 100000, DutyCycle::e6CPH );
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "pv=%g, ontime=%d, offtime=%d", pv, ontime, offtime ) );
        REQUIRE( ontime == 100000 );
        REQUIRE( ontime < offtime );

        pv      = 99.0F;
        ontime  = DutyCycle::calculateOnTime( pv, 100000, DutyCycle::e6CPH );
        offtime = DutyCycle::calculateOffTime( pv, 100000, DutyCycle::e6CPH );
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "pv=%g, ontime=%d, offtime=%d ", pv, ontime, offtime ) );
        REQUIRE( offtime == 100000 );
        REQUIRE( ontime > offtime );
    }

    SECTION( "Range" )
    {
        float    pv      = 125.0F;
        uint32_t ontime  = DutyCycle::calculateOnTime( pv, 0, DutyCycle::e6CPH, 100.0F, 150.0F );
        uint32_t offtime = DutyCycle::calculateOffTime( pv, 0, DutyCycle::e6CPH, 100.0F, 150.0F );
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "pv=%g, ontime=%d, offtime=%d", pv, ontime, offtime ) );
        REQUIRE( ontime == offtime );

        pv      = 100.0F ;
        ontime  = DutyCycle::calculateOnTime( pv, 0, DutyCycle::e6CPH, 0.0F, 200.0F );
        offtime = DutyCycle::calculateOffTime( pv, 0, DutyCycle::e6CPH, 0.0F, 200.0F );
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "pv=%g, ontime=%d, offtime=%d ", pv, ontime, offtime ) );
        REQUIRE( ontime == offtime );

        pv      = 125.0F;
        ontime  = DutyCycle::calculateOnTime( pv, 0, DutyCycle::e6CPH, 0.0F, 100.0F );
        offtime = DutyCycle::calculateOffTime( pv, 0, DutyCycle::e6CPH, 0.0F, 100.0F );
        CPL_SYSTEM_TRACE_MSG( SECT_, ( "pv=%g, ontime=%d, offtime=%d ", pv, ontime, offtime ) );
        REQUIRE( ontime == 150000 );
        REQUIRE( offtime == 0 );
    }

    REQUIRE( Cpl::System::Shutdown_TS::getAndClearCounter() == 0u );
}

