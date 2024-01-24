#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <sstream>

#include "waypoint.h"
#include "nand/nand-data_entry.h"
#include "nand/nand-parser.h"

using namespace GPS;
using namespace NAND;

BOOST_AUTO_TEST_SUITE( ProcessDataLog )

const double percentageAccuracy = 0.0001;

const std::string validNEILdata = "~NEIL|+45.67,-23.24,231.56|028;";
const std::string validALISdata = "~ALIS|78o36'45'',N,23o42'56'',W,23.62|061;";
const std::string validNUNOdata = "~NUNO|2000-01-11T01:10:05Z,56.89,-17.5,+51.4|013;";
const std::string validDAVEdata = "~DAVE|09:33:21,78o36'45'',S,23o13'56'',E,56.89|023;";

const GPS::Waypoint waypointNEIL = GPS::Waypoint(45.67,-23.24,231.56);
const GPS::Waypoint waypointALIS = GPS::Waypoint(78.6125,-23.715556,23.62);
const GPS::Waypoint waypointNUNO = GPS::Waypoint(51.4,-17.5,56.89);
const GPS::Waypoint waypointDAVE = GPS::Waypoint(-78.6125,23.232222,56.89);

BOOST_AUTO_TEST_CASE( EmptyLog )
{
    std::stringstream dataLog("");
    const unsigned int expectedSize = 0;

    std::vector<Waypoint> waypoints = processDataLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( OneValidEntry )
{
    std::stringstream dataLog;
    dataLog << validNEILdata;
    const unsigned int expectedSize = 1;
    const Waypoint expectedWaypoint = waypointNEIL;

    std::vector<Waypoint> waypoints = processDataLog(dataLog);

    BOOST_REQUIRE_EQUAL( waypoints.size() , expectedSize );
    const Waypoint actualWaypoint = waypoints.front();
    BOOST_CHECK_CLOSE( actualWaypoint.latitude(), expectedWaypoint.latitude(), percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude(), expectedWaypoint.longitude(), percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude(), expectedWaypoint.altitude(), percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( FourValidEntries )
{
    std::stringstream dataLog;
    dataLog << validNEILdata << validALISdata << validNUNOdata << validDAVEdata;
    const std::vector<Waypoint> expectedWaypoints = { waypointNEIL, waypointALIS, waypointNUNO, waypointDAVE};

    std::vector<Waypoint> actualWaypoints = processDataLog(dataLog);

    BOOST_REQUIRE_EQUAL( actualWaypoints.size() , expectedWaypoints.size() );
    for (unsigned int i = 0; i < expectedWaypoints.size(); ++i)
    {
        BOOST_CHECK_CLOSE( actualWaypoints[i].latitude(), expectedWaypoints[i].latitude(), percentageAccuracy );
        BOOST_CHECK_CLOSE( actualWaypoints[i].longitude(), expectedWaypoints[i].longitude(), percentageAccuracy );
        BOOST_CHECK_CLOSE( actualWaypoints[i].altitude(), expectedWaypoints[i].altitude(), percentageAccuracy );
    }
}

BOOST_AUTO_TEST_CASE( IgnoringWhiteSpace )
{
    std::stringstream dataLog;
    dataLog << "\n\n" << validNEILdata << "    " << validALISdata << " \n" << validNUNOdata << "\t" << validDAVEdata << std::endl;
    const unsigned int expectedSize = 4;

    std::vector<Waypoint> waypoints = processDataLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( IllFormedNANDdataEntries )
{
    std::stringstream dataLog;
    dataLog << validNEILdata << "~2345;" << validALISdata << "~NUNO|23,A|;" << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = processDataLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( InvalidChecksums )
{
    const std::string invalidChecksumdataNEIL = "~NEIL|+45.67,-23.24,231.56|100;";
    const std::string invalidChecksumdataALIS = "~ALIS|78o36'45'',N,23o42'56'',W,23.62|074;";
    std::stringstream dataLog;
    dataLog << invalidChecksumdataNEIL << validDAVEdata << invalidChecksumdataALIS << validNUNOdata << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = processDataLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( IllformedStructureButValidChecksum )
{
    const std::string illformedNANDentry = "|A|065;";
    std::stringstream dataLog;
    dataLog << validALISdata << illformedNANDentry << validNEILdata << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = processDataLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( UnrecognisedNANDformats )
{
    const std::string unrecognisedNANDentry = "~ISMA|NNW,40mph,14:21:58|045;";
    std::stringstream dataLog;
    dataLog << validALISdata << unrecognisedNANDentry << validNEILdata << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = processDataLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( MissingFields )
{
    const std::string missingFieldsNEILdata = "~NEIL|+45.67,231.56|052;";
    const std::string missingFieldsALISdata = "~ALIS|78o36'45'',23o42'56'',23.62|036;";
    const std::string missingFieldsNUNOdata = "~NUNO|56.89,-17.5,+51.4|041;";
    const std::string missingFieldsDAVEdata = "~DAVE|09:33:21,78o36'45'',S,23o13'56'',E|023;";

    std::stringstream dataLog;
    dataLog << validALISdata << missingFieldsNEILdata << validNEILdata << missingFieldsALISdata
            << missingFieldsDAVEdata << validNUNOdata << missingFieldsNUNOdata << std::endl;
    const unsigned int expectedSize = 3;

    std::vector<Waypoint> waypoints = processDataLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( ExtraFields )
{
    const std::string extraFieldsNEILdata = "~NEIL|+45.67,-23.24,231.56,15/07/87|028;";
    const std::string extraFieldsALISdata = "~ALIS|23/11/06,78o36'45'',N,23o42'56'',W,23.62|061;";
    const std::string extraFieldsNUNOdata = "~NUNO|2000-01-11T01:10:05Z,56.89,-17.5,+51.4,A|013;";
    const std::string extraFieldsDAVEdata = "~DAVE|09:33:21,30/06/17,78o36'45'',S,23o13'56'',E,56.89|023;";

    std::stringstream dataLog;
    dataLog << validALISdata << extraFieldsNEILdata << validNEILdata << extraFieldsALISdata
            << extraFieldsDAVEdata << validNUNOdata << extraFieldsNUNOdata << std::endl;
    const unsigned int expectedSize = 3;

    std::vector<Waypoint> waypoints = processDataLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( InvalidFields )
{
    const std::string invalidFieldsNEILdata = "~NEIL|+97.67,-23.24,231.56|019;"; // latitude > 90o
    const std::string invalidFieldsNUNOdata = "~NUNO|2000-01-11T01:10:05Z,126.89,-274.5,+51.4|012;"; // longitude > 180o
    const std::string invalidFieldsALISdata = "~ALIS|7836'45'',N,23o42'56'',W,23.62|082;"; // missing o
    const std::string invalidFieldsDAVEdata = "~DAVE|09:33:21,78o36'45'',S,23o13'56,E,56.89|023;"; // missing ''

    std::stringstream dataLog;
    dataLog << validALISdata << invalidFieldsNEILdata << validNEILdata << invalidFieldsALISdata
            << invalidFieldsDAVEdata << validNUNOdata << invalidFieldsNUNOdata << std::endl;
    const unsigned int expectedSize = 3;

    std::vector<Waypoint> waypoints = processDataLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_SUITE_END()
