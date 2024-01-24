#ifndef GPS_NAND_DATAENTRY_H
#define GPS_NAND_DATAENTRY_H

#include <string>
#include <vector>

namespace GPS::NAND
{

// Stores the format code and fields of a NAND data entry (the checksum is not stored).
struct DataEntry
{
    /* Stores the NAND format code.
     * E.g. "NEIL".
     */
    std::string format;

    /* Stores the data fields.
     * E.g.
     *  - the first element of the vector could be "+45.34",
     *  - the second element could be "-32.67".
     */
    std::vector<std::string> dataFields;
};

}

#endif
