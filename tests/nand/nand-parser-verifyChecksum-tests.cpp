#include <boost/test/unit_test.hpp>

#include <string>

#include "nand/nand-parser.h"

using namespace GPS;
using namespace NAND;

BOOST_AUTO_TEST_SUITE( VerifyChecksum )

BOOST_AUTO_TEST_CASE( SingleCharacterData )
{
    BOOST_CHECK( verifyChecksum("~XXXX|A|065;") );
}

BOOST_AUTO_TEST_CASE( TwoCharacterData )
{
    BOOST_CHECK( verifyChecksum("~XXXX|AB|003;") );
}

BOOST_AUTO_TEST_CASE( EmptyData )
{
    BOOST_CHECK( verifyChecksum("~XXXX||000;") );
}

BOOST_AUTO_TEST_CASE( TypicalData )
{
    BOOST_CHECK( verifyChecksum("~NEIL|+45.67,-23.24,231.56|028;") );
}

BOOST_AUTO_TEST_CASE( SingleCharacterDataCorrupt )
{
    BOOST_CHECK( ! verifyChecksum("~XXXX|A|112;") );
}

BOOST_AUTO_TEST_CASE( TypicalDataCorrupt )
{
    BOOST_CHECK( ! verifyChecksum("~NEIL|+45.67,-23.24,231.56|018;") );
}

BOOST_AUTO_TEST_SUITE_END()
