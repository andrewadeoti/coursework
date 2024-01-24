#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <stdexcept>

#include "waypoint.h"
#include "nand/nand-data_entry.h"
#include "nand/nand-parser.h"

using namespace GPS;
using namespace NAND;

BOOST_AUTO_TEST_SUITE( DataEntryToWaypoint )

const double percentageAccuracy = 0.0001;
const double absoluteAccuracy = 0.0001;

BOOST_AUTO_TEST_CASE( NEIL )
{
    const DataEntry dataEntry = { "NEIL", {"+45.67","-23.24","231.56"} };
    const degrees expectedLatitude = 45.67;
    const degrees expectedLongitude = -23.24;
    const degrees expectedAltitude = 231.56;

    Waypoint actualWaypoint = dataEntryToWaypoint(dataEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( NUNO )
{
    const DataEntry dataEntry = { "NUNO", {"2000-01-11T01:10:05Z","56.89","17.5","-51.4"} };
    const degrees expectedLatitude = -51.4;
    const degrees expectedLongitude = 17.5;
    const degrees expectedAltitude = 56.89;

    Waypoint actualWaypoint = dataEntryToWaypoint(dataEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( ALIS_NW )
{
    const DataEntry dataEntry = { "ALIS", {"78o36'45''","N","23o33'56''","W","23.62"} };
    const degrees expectedLatitude = 78.6125;
    const degrees expectedLongitude = -23.565556;
    const degrees expectedAltitude = 23.62;

    Waypoint actualWaypoint = dataEntryToWaypoint(dataEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( ALIS_SE )
{
    const DataEntry dataEntry = { "ALIS", {"22o47'03''","S","38o14'43''","E","467.21"} };
    const degrees expectedLatitude = -22.784167;
    const degrees expectedLongitude = 38.245278;
    const degrees expectedAltitude = 467.21;

    Waypoint actualWaypoint = dataEntryToWaypoint(dataEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( DAVE_NE )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","28o50'17''","N","42o8'24''","E","56.89"} };
    const degrees expectedLatitude = 28.838056;
    const degrees expectedLongitude = 42.14;
    const degrees expectedAltitude = 56.89;

    Waypoint actualWaypoint = dataEntryToWaypoint(dataEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( DAVE_SW )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","52o34'45''","S","11o27'36''","W","234.7"} };
    const degrees expectedLatitude = -52.579167;
    const degrees expectedLongitude = -11.46;
    const degrees expectedAltitude = 234.7;

    Waypoint actualWaypoint = dataEntryToWaypoint(dataEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( NegativeAltitude )
{
    const DataEntry dataEntry = { "NEIL", {"+45.67","-23.24","-456.8"} };
    const degrees expectedLatitude = 45.67;
    const degrees expectedLongitude = -23.24;
    const degrees expectedAltitude = -456.8;

    Waypoint actualWaypoint = dataEntryToWaypoint(dataEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( Longitude_above_90 )
{
    const DataEntry dataEntry = { "ALIS", {"78o36'45''","N","123o33'56''","W","23.62"} };
    const degrees expectedLatitude = 78.6125;
    const degrees expectedLongitude = -123.565556;
    const degrees expectedAltitude = 23.62;

    Waypoint actualWaypoint = dataEntryToWaypoint(dataEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( ZeroValues )
{
    const DataEntry dataEntry = { "NEIL", {"+0.0","-0.00","0.00"} };

    Waypoint actualWaypoint = dataEntryToWaypoint(dataEntry);

    BOOST_CHECK_SMALL( actualWaypoint.latitude() , absoluteAccuracy );
    BOOST_CHECK_SMALL( actualWaypoint.longitude() , absoluteAccuracy );
    BOOST_CHECK_SMALL( actualWaypoint.altitude() , absoluteAccuracy );
}

BOOST_AUTO_TEST_CASE( InvalidAltitudeText )
{
    const DataEntry dataEntry = { "NEIL", {"86.24","-23.24","in orbit"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLatitudeText )
{
    const DataEntry dataEntry = { "NEIL", {"far north","-23.24","231.56"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLongitudeText )
{
    const DataEntry dataEntry = { "NEIL", {"24.78","far east","231.56"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_negative_ALIS_lon )
{
    const DataEntry dataEntry = { "ALIS", {"22o47'03''","S","-38o14'26","E","467.21"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingDegreesSymbol_ALIS_lon )
{
    const DataEntry dataEntry = { "ALIS", {"22o47'03''","S","3814'26''","E","467.21"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingMinutesSymbol_ALIS_lon )
{
    const DataEntry dataEntry = { "ALIS", {"22o47'03''","S","38o1426''","E","467.21"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingSecondsSymbol_ALIS_lon )
{
    const DataEntry dataEntry = { "ALIS", {"22o47'03''","S","38o14'26","E","467.21"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_not_numeric_ALIS_lon )
{
    const DataEntry dataEntry = { "ALIS", {"22o47'03''","S","over there","E","467.21"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLonBearingSymbol_ALIS_lon )
{
    const DataEntry dataEntry = { "ALIS", {"22o47'03''","N","38o14'43''","Y","467.21"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_negative_ALIS_lat )
{
    const DataEntry dataEntry = { "ALIS", {"-22o47'03''","S","38o14'26","E","467.21"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingDegreesSymbol_ALIS_lat )
{
    const DataEntry dataEntry = { "ALIS", {"2247'03''","S","38o14'26''","E","467.21"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingMinutesSymbol_ALIS_lat )
{
    const DataEntry dataEntry = { "ALIS", {"22o4703''","S","38o14'26''","E","467.21"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingSecondsSymbol_ALIS_lat )
{
    const DataEntry dataEntry = { "ALIS", {"22o47'03","S","38o14'26''","E","467.21"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_not_numeric_ALIS_lat )
{
    const DataEntry dataEntry = { "ALIS", {"up here","S","38o14'26''","E","467.21"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLonBearingSymbol_ALIS_lat )
{
    const DataEntry dataEntry = { "ALIS", {"22o47'03''","X","38o14'43''","E","467.21"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_negative_DAVE_lon )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","28o50'17''","N","-42o8'24''","E","56.89"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingDegreesSymbol_DAVE_lon )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","28o50'17''","N","428'24''","E","56.89"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingMinutesSymbol_DAVE_lon )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","28o50'17''","N","42o824''","E","56.89"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingSecondsSymbol_DAVE_lon )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","28o50'17''","N","42o8'24'","E","56.89"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_not_numeric_DAVE_lon )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","28o50'17''","N","nowhere","E","56.89"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLonBearingSymbol_DAVE_lon )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","28o50'17''","N","42o8'24''","?","56.89"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_negative_DAVE_lat )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","-28o50'17''","N","42o8'24''","E","56.89"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingDegreesSymbol_DAVE_lat )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","2850'17''","N","42o8'24''","E","56.89"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingMinutesSymbol_DAVE_lat )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","28o5017''","N","42o8'24''","E","56.89"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingSecondsSymbol_DAVE_lat )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","28o50'17'","N","42o8'24''","E","56.89"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_not_numeric_DAVE_lat )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","-","N","42o8'24''","E","56.89"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLonBearingSymbol_DAVE_lat )
{
    const DataEntry dataEntry = { "DAVE", {"09:33:21","28o50'17''"," ","42o8'24''","E","56.89"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidBearingCase )
{
    const DataEntry dataEntry = { "ALIS", {"22o47'03''","n","38o14'43''","e","467.21"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidBearingWithValidPrefix )
{
    const DataEntry dataEntry = { "ALIS", {"22o47'03''","Nowhere","38o14'43''","Wherever","467.21"} };

    BOOST_CHECK_THROW( dataEntryToWaypoint(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_SUITE_END()
