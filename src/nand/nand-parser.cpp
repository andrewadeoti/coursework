// #include <iostream> // For debugging
#include <stdexcept>
#include <string>
#include "nand/nand-parser.h"
namespace GPS::NAND
{

        bool isRecognisedCode(const std::string& code) {
            // Check if the code is of valid length
            if (code.length() != 4) {
                return false;
            }

            // Check if all characters are uppercase
            for (char c : code) {
                if (!std::isupper(c)) {
                    return false;
                }
            }

            // Check if the code is one of the recognized codes
            return (code == "NEIL" || code == "ALIS" || code == "DAVE" || code == "NUNO");
        }
}


  bool hasFormOfNANDdataEntry(std::string s)
  {
      char c;
      unsigned int i;
      if (s.empty()) return false; // Need this or next line could crash
      if (s[0] != '~') return false;
      if (s.size() < 5) return false; // Need this or next line could crash
      // cout << "Debug A" << endl;
      c = s[1];
      if (c < 65 || c > 90) return false;
      c = s[2];
      if (c < 65 || c > 90) return false;
      c = s[3];
      if (c < 65 || c > 90) return false;
      c = s[4];
      if (c < 'A' || c > 'Z') return false; // Hey Ken: we can do it like this instead of using the ASCII codes.  Shall I change the previous two like this as well?
      // cout << "Debug B" << endl;
      if (s.size() < 6) return false; // Need this or next line could crash.
      if (s[5] != '|') return false;
      // cout << "Debug C" << endl;
      /*
      i = 6;
      while (true)
      {
          if (i == s.size()) return false;
          if (s[i] == '~') return false; // Not allowed here.
          if (s[i] == ';') break;
          ++i;
      }
      */
      // Nadia: for-loop is better.
      for (i = 6; true; ++i)
      {
          if (i == s.size()) return false;
          if (s[i] == '~') return false; // Not allowed here.
          if (s[i] == ';') return false; // Not allowed here.
          if (s[i] == '|') break;
      }
      // cout << "Debug D" << endl;
      if (s.size() < i+4) return false; // Note: i+4 NOT i+3.  This is confusing.
      c = s[i+1];
      if ((c < 48) || (c > 57)) return false;
      c = s[i+2];
      if ((c < 48) || (c > 57)) return false;
      c = s[i+3];
      if ((c < 48) || (c > 57)) return false;
      // cout << "Debug E" << endl;
      // cout << s.size() << endl;
      // cout << i << endl;
      if (s[i+4] != ';') return false;
      if (s.size() != i+5) return false; // Note: i+5 NOT i+4.  This is confusing.
      // Alice: It's because indexing starts at 0, so the index number is one less.
      // Nadia: No, it's because we're using '<' rather than '<='.  We need a char at i+4, so we say < i+5.
      // Ken: But indexing normally starts at 0, and we normally use <. So why is this different?
      return true;
  }
  bool verifyChecksum(std::string input)
  {
      if (input.empty() || input.size() < 11 || input[0] != '~' || input[5] != '|' || input[input.size() - 1] != ';')
      {
          return false;
      }

      // Extract the checksum part
      std::string checksumStr = input.substr(input.size() - 4, 3);

      try
      {
          int checksum = std::stoi(checksumStr);

          // Calculate the expected checksum
          int expectedChecksum = 0;
          for (size_t i = 1; i < input.size() - 5; ++i)
          {
              expectedChecksum += static_cast<int>(input[i]);
          }

          // Compare the calculated checksum with the expected checksum
          return checksum == expectedChecksum % 1000;
      }
      catch (const std::invalid_argument&)
      {
          // Conversion to integer failed
          return false;
      }
  }
  NAND::DataEntry parseDataEntry(std::string)
  {
      // Stub definition, needs implementing
      return {"",};
  }
  bool hasExpectedNumberOfFields(const DataEntry&dataEntry) {
      const std::string& code = dataEntry.code;
      const std::vector<std::string>& fields = dataEntry.fields;

      if (code == "NEIL" && fields.size() == 3) {
          return true;
      } else if (code == "ALIS" && fields.size() == 5) {
          return true;
      } else if (code == "NUNO" && fields.size() == 4) {
          return true;
      } else if (code == "DAVE" && fields.size() == 6) {
          return true;
      }

      return false;
  }
  Waypoint dataEntryToWaypoint(NAND::DataEntry d)
  {
      using namespace std; // Ken: Writing 'std::' everywhere is irritating.
      string la, lo, al, ns, ew, de, mi, se, temp;
      double degs, mins, secs, lat, lon, alt;
      bool onPrime, dsym, msym, ssym;
      Waypoint w = Waypoint(0,0,0); // Dummy object becasue there's no default constructor available for Waypoint
      if (d.format == "NEIL")
      {
          la = d.dataFields[0];
          lo = d.dataFields[1];
          al = d.dataFields[2];
          try
          {
              lat = stod(la);
              lon = stod(lo);
              alt = stod(al);
              w = Waypoint(lat,lon,alt);
          }
          catch (const invalid_argument& e)
          {
              throw domain_error(string("Ill-formed NEIL data field: ") + e.what());
          }
      }
      else if (d.format == "ALIS")
      {
          la = d.dataFields[0];
          ns = d.dataFields[1];
          lo = d.dataFields[2];
          ew = d.dataFields[3];
          al = d.dataFields[4];
          try
          {
              onPrime = false;
              dsym = false;
              msym = false;
              ssym = false;
              for (unsigned int i = 0; i < la.length(); ++i)
              {
                  if (onPrime == true)
                  {
                      if (la[i] != '\'')
                      {
                          msym = true;
                          mi = temp;
                          temp = "";
                          temp += la[i];
                          onPrime = false;
                      }
                      else
                      {
                          ssym = true;
                          se = temp;
                      }
                  }
                  else if (la[i] == 'o')
                  {
                      dsym = true;
                      de = temp;
                      temp = "";
                  }
                  else if (la[i] == '\'')
                  {
                      if (onPrime == true)
                      {
                      }
                      else
                      {
                          onPrime = true;
                      }
                  }
                  else
                  {
                      temp += la[i];
                  }
              }
              if (!dsym || !msym || ! ssym)
              {
                  throw std::domain_error("missing DMS symbols in data field: " + la);
              }
              degs = stod(de);
              mins = stod(mi);
              secs = stod(se);
              lat = degs + mins/60 + secs/3600;
              if (lat < 0)
              {
                  throw std::invalid_argument("latitude values in DMS format must be positive.  Positive/negative direction is denoted by a separate bearing indicator.");
              }
              temp = "";
              onPrime = false;
              dsym = false;
              msym = false;
              ssym = false;
              for (unsigned int i = 0; i < lo.length(); ++i)
              {
                  if (onPrime == true)
                  {
                      if (lo[i] != '\'')
                      {
                          msym = true;
                          mi = temp;
                          temp = "";
                          temp += lo[i];
                          onPrime = false;
                      }
                      else
                      {
                          ssym = true;
                          se = temp;
                      }
                  }
                  else if (lo[i] == 'o')
                  {
                      dsym = true;
                      de = temp;
                      temp = "";
                  }
                  else if (lo[i] == '\'')
                  {
                      if (onPrime == true)
                      {
                      }
                      else
                      {
                          onPrime = true;
                      }
                  }
                  else
                  {
                      temp += lo[i];
                  }
              }
              if (!dsym || !msym || ! ssym)
              {
                  throw std::invalid_argument("missing DMS symbols in data field: " + la);
              }
              degs = stod(de);
              mins = stod(mi);
              secs = stod(se);
              lon = degs + mins/60 + secs/3600;
              if (lon < 0)
              {
                  throw std::invalid_argument("longitude values in DMS format must be positive.  Positive/negative direction is denoted by a separate bearing indicator.");
              }
              alt = stod(al);
          }
          catch (const invalid_argument& e)
          {
              throw domain_error(string("Ill-formed ALIS data field: ") + e.what());
          }
          if (ns.size() == 1) {
              switch (ns[0])
              {
                  case 'N': break;
                  case 'S': lat = -lat; break;
                  default: throw domain_error("Ill-formed ALIS data field: " + ns + " is an invalid latitude bearing indicator.  Only 'N' or 'S' accepted.");
              }
          } else {
              throw domain_error("Ill-formed ALIS data field: " + ns + " is an invalid latitude bearing indicator.  Only 'N' or 'S' accepted.");
          }
          if (ew.size() == 1) {
              switch (ew[0])
              {
                  case 'E': break;
                  case 'W': lon = -lon; break;
                  default: throw domain_error("Ill-formed ALIS data field: " + ew + " is an invalid longitude bearing indicator.  Only 'E' or 'W' accepted.");
              }
          } else {
              throw domain_error("Ill-formed ALIS data field: " + ew + " is an invalid longitude bearing indicator.  Only 'E' or 'W' accepted.");
          }
          try
          {
                w = Waypoint(lat,lon,alt);
          }
          catch (const invalid_argument& e)
          {
                throw domain_error(string("Ill-formed ALIS data field: ") + e.what());
          }
      }
      else if (d.format == "NUNO")
      {
          la = d.dataFields[3];
          lo = d.dataFields[2];
          al = d.dataFields[1];
          try
          {
              lat = stod(la);
              lon = stod(lo);
              alt = stod(al);
          }
          catch (const invalid_argument& e)
          {
              throw domain_error(string("Ill-formed NUNO data field: ") + e.what());
          }
          try
          {
              w = Waypoint(lat,lon,alt);
          }
          catch (const invalid_argument& e)
          {
              throw domain_error(string("Ill-formed NUNO data field: ") + e.what());
          }
      }
      else if (d.format == "DAVE")
      {
          la = d.dataFields[1];
          ns = d.dataFields[2];
          lo = d.dataFields[3];
          ew = d.dataFields[4];
          al = d.dataFields[5];
          try
          {
              onPrime = false;
              dsym = false;
              msym = false;
              ssym = false;
              for (unsigned int i = 0; i < la.length(); ++i)
              {
                  if (onPrime == true)
                  {
                      if (la[i] != '\'')
                      {
                          msym = true;
                          mi = temp;
                          temp = "";
                          temp += la[i];
                          onPrime = false;
                      }
                      else
                      {
                          ssym = true;
                          se = temp;
                      }
                  }
                  else if (la[i] == 'o')
                  {
                      dsym = true;
                      de = temp;
                      temp = "";
                  }
                  else if (la[i] == '\'')
                  {
                      if (onPrime == true)
                      {
                      }
                      else
                      {
                          onPrime = true;
                      }
                  }
                  else
                  {
                      temp += la[i];
                  }
              }
              if (!dsym || !msym || ! ssym)
              {
                  throw std::domain_error("missing DMS symbols in data field: " + la);
              }
              degs = stod(de);
              mins = stod(mi);
              secs = stod(se);
              lat = degs + mins/60 + secs/3600;
              if (lat < 0)
              {
                  throw std::invalid_argument("latitude values in DMS format must be positive.  Positive/negative direction is denoted by a separate bearing indicator.");
              }
              temp = "";
              onPrime = false;
              dsym = false;
              msym = false;
              ssym = false;
              for (unsigned int i = 0; i < lo.length(); ++i)
              {
                  if (onPrime == true)
                  {
                      if (lo[i] != '\'')
                      {
                          msym = true;
                          mi = temp;
                          temp = "";
                          temp += lo[i];
                          onPrime = false;
                      }
                      else
                      {
                          ssym = true;
                          se = temp;
                      }
                  }
                  else if (lo[i] == 'o')
                  {
                      dsym = true;
                      de = temp;
                      temp = "";
                  }
                  else if (lo[i] == '\'')
                  {
                      if (onPrime == true)
                      {
                      }
                      else
                      {
                          onPrime = true;
                      }
                  }
                  else
                  {
                      temp += lo[i];
                  }
              }
              if (!dsym || !msym || ! ssym)
              {
                  throw std::invalid_argument("missing DMS symbols in data field: " + la);
              }
              degs = stod(de);
              mins = stod(mi);
              secs = stod(se);
              lon = degs + mins/60 + secs/3600;
              if (lon < 0)
              {
                  throw std::invalid_argument("longitude values in DMS format must be positive.  Positive/negative direction is denoted by a separate bearing indicator.");
              }
              alt = stod(al);
          }
          catch (const invalid_argument& e)
          {
              throw domain_error(string("Ill-formed DAVE data field: ") + e.what());
          }
          if (ns.size() == 1) {
              switch (ns[0])
              {
                  case 'N': break;
                  case 'S': lat = -lat; break;
                  default: throw domain_error("Ill-formed DAVE data field: " + ns + " is an invalid latitude bearing indicator.  Only 'N' or 'S' accepted.");
              }
          } else {
              throw domain_error("Ill-formed DAVE data field: " + ns + " is an invalid latitude bearing indicator.  Only 'N' or 'S' accepted.");
          }
          if (ew.size() == 1) {
              switch (ew[0])
              {
                  case 'E': break;
                  case 'W': lon = -lon; break;
                  default: throw domain_error("Ill-formed DAVE data field: " + ew + " is an invalid longitude bearing indicator.  Only 'E' or 'W' accepted.");
              }
          } else {
              throw domain_error("Ill-formed DAVE data field: " + ew + " is an invalid longitude bearing indicator.  Only 'E' or 'W' accepted.");
          }
          try
          {
                w = Waypoint(lat,lon,alt);
          }
          catch (const invalid_argument& e)
          {
                throw domain_error(string("Ill-formed DAVE data field: ") + e.what());
          }
      }
      return w;
  }
  std::vector<Waypoint> processDataLog(std::istream&)
  {
      // Stub definition, needs implementing
      return {};
  }
}
