#include <boost/test/unit_test.hpp>

#include <string>

#include "nand/nand-parser.h"

using namespace GPS;
using namespace NAND;

BOOST_AUTO_TEST_SUITE( IsRecognisedCode )

BOOST_AUTO_TEST_CASE( NEIL_IsRecognisedCode )
{
    BOOST_CHECK( isRecognisedCode("NEIL") );
}

BOOST_AUTO_TEST_CASE( ALIS_IsRecognisedCode )
{
    BOOST_CHECK( isRecognisedCode("ALIS") );
}

BOOST_AUTO_TEST_CASE( DAVE_IsRecognisedCode )
{
    BOOST_CHECK( isRecognisedCode("DAVE") );
}

BOOST_AUTO_TEST_CASE( NUNO_IsRecognisedCode )
{
    BOOST_CHECK( isRecognisedCode("NUNO") );
}

BOOST_AUTO_TEST_CASE( UnrecognisedCodes )
{
    BOOST_CHECK( ! isRecognisedCode("ISMA") );
    BOOST_CHECK( ! isRecognisedCode("AMIR") );
}

BOOST_AUTO_TEST_CASE( UncapitalisedCodes )
{
    BOOST_CHECK( ! isRecognisedCode("Neil") );
    BOOST_CHECK( ! isRecognisedCode("nuno") );
}

BOOST_AUTO_TEST_CASE( CodesWrongLength )
{
    BOOST_CHECK( ! isRecognisedCode("") );
    BOOST_CHECK( ! isRecognisedCode("D") );
    BOOST_CHECK( ! isRecognisedCode("NU") );
    BOOST_CHECK( ! isRecognisedCode("ALISIA") ); // prefix is valid format
    BOOST_CHECK( ! isRecognisedCode("ONEIL") ); // suffix is valid format
}

BOOST_AUTO_TEST_CASE( CodesVeryLong )
{
    const std::string veryLongString(10000,'X');
    BOOST_CHECK( ! isRecognisedCode(veryLongString) );
}

BOOST_AUTO_TEST_SUITE_END()
