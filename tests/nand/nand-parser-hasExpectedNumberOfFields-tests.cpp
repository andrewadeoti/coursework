#include <boost/test/unit_test.hpp>

#include <string>

#include "nand/nand-data_entry.h"
#include "nand/nand-parser.h"

using namespace GPS;
using namespace NAND;

BOOST_AUTO_TEST_SUITE( HasExpectedNumberOfFields )

BOOST_AUTO_TEST_CASE( ExpectedFieldsNEIL )
{
    const DataEntry dataEntry = { "NEIL", {"+45.67","-23.24","231.56"} };

    BOOST_CHECK( hasExpectedNumberOfFields(dataEntry) );
}

BOOST_AUTO_TEST_CASE( ExpectedFieldsALIS )
{
    const DataEntry dataEntry = { "ALIS", {"78o36'45''","N","23o42'56''","W","23.62"} };

    BOOST_CHECK( hasExpectedNumberOfFields(dataEntry) );
}

BOOST_AUTO_TEST_CASE( ExpectedFieldsNUNO )
{
    const DataEntry dataEntry = { "NUNO", {"2000-01-11T01:10:05Z","56.89","-17.5","+51.4"} };

    BOOST_CHECK( hasExpectedNumberOfFields(dataEntry) );
}

BOOST_AUTO_TEST_CASE( ExpectedFieldsDAVE )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","78o36'45''","N","23o13'56''","E","56.89"} };

    BOOST_CHECK( hasExpectedNumberOfFields(dataEntry) );
}

BOOST_AUTO_TEST_CASE( MissingFields )
{
    const DataEntry dataEntry = { "NUNO", {"56.89","-17.5"} };

    BOOST_CHECK( ! hasExpectedNumberOfFields(dataEntry) );
}

BOOST_AUTO_TEST_CASE( ExtraFields )
{
    const DataEntry dataEntry = { "ALIS", {"78o36'45''","N","23o42'56''","E","23.62","17:43:07","14/08/74"} };

    BOOST_CHECK( ! hasExpectedNumberOfFields(dataEntry) );
}

BOOST_AUTO_TEST_CASE( ZeroFields )
{
    const DataEntry dataEntry = { "NEIL", {} };

    BOOST_CHECK( ! hasExpectedNumberOfFields(dataEntry) );
}

BOOST_AUTO_TEST_SUITE_END()
