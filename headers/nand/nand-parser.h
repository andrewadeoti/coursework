#ifndef GPS_NAND_PARSER_H
#define GPS_NAND_PARSER_H

#include <string>
#include <vector>
#include <istream>

#include "nand-data_entry.h"
#include "waypoint.h"

namespace GPS::NAND
{
  bool isRecognisedCode(std::string);


  bool hasFormOfNANDdataEntry(std::string);


  // Pre-condition: the argument string has the form of a NAND data entry.
  bool verifyChecksum(std::string);


  // Pre-condition: the argument string has the form of a NAND data entry.
  NAND::DataEntry parseDataEntry(std::string);


  // Pre-condition: the data entry contains a recognised format code.
  bool hasExpectedNumberOfFields(NAND::DataEntry);


  // Pre-conditions:
  //    - the data entry contains a recognised format code.
  //    - the data entry has the expected number of fields.
  GPS::Waypoint dataEntryToWaypoint(NAND::DataEntry);


  std::vector<GPS::Waypoint> processDataLog(std::istream&);
}

#endif
