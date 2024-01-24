#include <boost/test/unit_test.hpp>

#include <string>

#include "nand/nand-parser.h"

using namespace GPS;
using namespace NAND;

BOOST_AUTO_TEST_SUITE( HasFormOfNANDdataEntry )

BOOST_AUTO_TEST_CASE( WellFormedTypicalDataEntries )
{
    BOOST_CHECK( hasFormOfNANDdataEntry("~NEIL|+45.67,-23.24,231.56|105;") );
    BOOST_CHECK( hasFormOfNANDdataEntry("~ALIS|78o36'45'',N,23o42'56'',W,23.62|067;") );
    BOOST_CHECK( hasFormOfNANDdataEntry("~NUNO|2000-01-11T01:10:05Z,56.89,-17.5,+51.4|023;") );
    BOOST_CHECK( hasFormOfNANDdataEntry("~DAVE|09:33:21,78o36'45'',N,23o13'56'',E,56.89|101;") );
}

BOOST_AUTO_TEST_CASE( InvalidStartSymbol )
{
    BOOST_CHECK( ! hasFormOfNANDdataEntry("!NEIL|+45.67,-23.24,231.56|105;") );
}

BOOST_AUTO_TEST_CASE( MissingStartSymbol )
{
    BOOST_CHECK( ! hasFormOfNANDdataEntry("NEIL|+45.67,-23.24,231.56|105;") );
}

BOOST_AUTO_TEST_CASE( MissingEndSymbol )
{
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~NEIL|+45.67,-23.24,231.56|105") );
}

BOOST_AUTO_TEST_CASE( MissingFormatCode )
{
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~|+45.67,-23.24,231.56|105;") );
}

BOOST_AUTO_TEST_CASE( MissingChecksum )
{
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~NEIL|+45.67,-23.24,231.56|;") );
}

BOOST_AUTO_TEST_CASE( InvalidChecksumLength )
{
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~NEIL|+45.67,-23.24,231.56|1;") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~NEIL|+45.67,-23.24,231.56|12;") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~NEIL|+45.67,-23.24,231.56|1234;") );
}

BOOST_AUTO_TEST_CASE( WellFormedOneField )
{
    BOOST_CHECK( hasFormOfNANDdataEntry("~XXXX|77|100;") );
}

BOOST_AUTO_TEST_CASE( WellFormedTwoFields )
{
    BOOST_CHECK( hasFormOfNANDdataEntry("~XXXX|77,A|087;") );
}

BOOST_AUTO_TEST_CASE( WellFormedEmptyField )
{
    BOOST_CHECK( hasFormOfNANDdataEntry("~XXXX||000;") );
}

BOOST_AUTO_TEST_CASE( WellFormedManyFields )
{
    const std::string commas(1000,','); // 1000 fields
    BOOST_CHECK( hasFormOfNANDdataEntry("~XXXX|" + commas + "|098;") );
}

BOOST_AUTO_TEST_CASE( WellFormedCodeEdgeLetters )
{
    BOOST_CHECK( hasFormOfNANDdataEntry("~AAAA|77|100;") );
    BOOST_CHECK( hasFormOfNANDdataEntry("~ZZZZ|77|100;") );
}

BOOST_AUTO_TEST_CASE( WellFormedChecksumEdgeDigits )
{
    BOOST_CHECK( hasFormOfNANDdataEntry("~XXXX|77|000;") );
    BOOST_CHECK( hasFormOfNANDdataEntry("~XXXX|77|999;") );
}

BOOST_AUTO_TEST_CASE( MissingSuffix )
{
    BOOST_CHECK( ! hasFormOfNANDdataEntry("") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~XXXX") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~XXXX|") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~XXXX|23") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~XXXX|23,") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~XXXX|23,A") );
}

BOOST_AUTO_TEST_CASE( IllformedFormatCode )
{
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~ALI|77|100;") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~DAVID|77|100;") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~1234|77|100;") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~$%&*|77|100;") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~NUNO |77|100;") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~neil|77|100;") );
}

BOOST_AUTO_TEST_CASE( InvalidReservedCharInField )
{
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~XXXX|23,~,A|100;") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~XXXX|23,;,A|100;") );
    BOOST_CHECK( ! hasFormOfNANDdataEntry("~XXXX|23,|,A|100;") );
}

BOOST_AUTO_TEST_SUITE_END()
