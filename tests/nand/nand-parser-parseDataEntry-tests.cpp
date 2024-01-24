#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <ostream>
#include <sstream>

#include "nand/nand-data_entry.h"
#include "nand/nand-parser.h"

using namespace GPS;
using namespace NAND;

BOOST_AUTO_TEST_SUITE( ParseDataEntry )

// Utility function for comparing DataEntrys for equality.
void checkDataEntryEqual(const NAND::DataEntry & actual, const NAND::DataEntry & expected);


BOOST_AUTO_TEST_CASE( NEIL )
{
    const std::string nandStr = "~NEIL|+45.67,-23.24,231.56|105;";
    const DataEntry expectedData = { "NEIL", {"+45.67","-23.24","231.56"} };

    DataEntry actualData = parseDataEntry(nandStr);

    checkDataEntryEqual(actualData , expectedData);
}

BOOST_AUTO_TEST_CASE( ALIS )
{
    const std::string nandStr = "~ALIS|78o36'45'',N,23o42'56'',E,23.62|067;";
    const DataEntry expectedData = { "ALIS", {"78o36'45''","N","23o42'56''","E","23.62"} };

    DataEntry actualData = parseDataEntry(nandStr);

    checkDataEntryEqual(actualData , expectedData);
}

BOOST_AUTO_TEST_CASE( NUNO )
{
    const std::string nandStr = "~NUNO|2000-01-11T01:10:05Z,56.89,-17.5,+51.4|023;";
    const DataEntry expectedData = { "NUNO", {"2000-01-11T01:10:05Z","56.89","-17.5","+51.4"} };

    DataEntry actualData = parseDataEntry(nandStr);

    checkDataEntryEqual(actualData , expectedData);
}

BOOST_AUTO_TEST_CASE( DAVE )
{
    const std::string nandStr = "~DAVE|09:33:21,78o36'45'',N,23o13'56'',W,56.89|101;";
    const DataEntry expectedData = { "DAVE", {"09:33:21","78o36'45''","N","23o13'56''","W","56.89"} };

    DataEntry actualData = parseDataEntry(nandStr);

    checkDataEntryEqual(actualData , expectedData);
}

BOOST_AUTO_TEST_CASE( ISMA_unrecognisedFormat )
{
    const std::string nandStr = "~ISMA|NNW,40mph,14:21:58|045;";
    const DataEntry expectedData = { "ISMA", {"NNW","40mph","14:21:58"} };

    DataEntry actualData = parseDataEntry(nandStr);

    checkDataEntryEqual(actualData , expectedData);
}

BOOST_AUTO_TEST_CASE( JustOneField )
{
    const std::string nandStr = "~XXXX|abc|074;";
    const DataEntry expectedData = { "XXXX", {"abc"} };

    DataEntry actualData = parseDataEntry(nandStr);

    checkDataEntryEqual(actualData , expectedData);
}

BOOST_AUTO_TEST_CASE( SomeEmptyFields )
{
    const std::string nandStr = "~XXXX|abc,,20,|000;";
    const DataEntry expectedData = { "XXXX", { "abc", "", "20", "" } };

    DataEntry actualData = parseDataEntry(nandStr);

    checkDataEntryEqual(actualData , expectedData);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Displaying vectors directly using the built-in BOOST macros is tricky.
 * For reasons I don't fully understand, just overloading operator<< here doesn't work.
 * See: https://stackoverflow.com/questions/17572583/boost-check-fails-to-compile-operator-for-custom-types
 *
 * So instead we introduce an auxilliary function for formatting the error message for mismatched vectors.
 */

std::ostream& operator<<(std::ostream& outputStream, const std::vector<std::string> & vec)
{
    outputStream << '{';
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) outputStream << ',';
        outputStream << '"' << *it << '"';
    }
    outputStream << '}';
    return outputStream;
}

std::string formatMismatchedFormatCode(std::string actualCode, std::string expectedCode)
{
    std::stringstream outputMessage;
    outputMessage << "parseDataEntry() has failed, mismatched format code [ \"" << actualCode << "\" != \"" << expectedCode << "\" ]";
    return outputMessage.str();
}

std::string formatMismatchedFieldData(const std::vector<std::string> & actualFields,
                                      const std::vector<std::string> & expectedFields)
{
    std::stringstream outputMessage;
    outputMessage << "parseDataEntry() has failed, mismatched fields [ " << actualFields << " != " << expectedFields << " ]";
    return outputMessage.str();
}

void checkDataEntryEqual(const NAND::DataEntry & actual, const NAND::DataEntry & expected)
{
    BOOST_CHECK_MESSAGE( actual.format == expected.format,
                         formatMismatchedFormatCode(actual.format, expected.format) );

    BOOST_CHECK_MESSAGE( actual.dataFields == expected.dataFields,
                         formatMismatchedFieldData(actual.dataFields,expected.dataFields) );
}

BOOST_AUTO_TEST_SUITE_END()
