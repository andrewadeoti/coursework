#ifndef GPS_EARTH_H
#define GPS_EARTH_H

#include "waypoint.h"

namespace GPS
{
  namespace Earth
  {
      extern const Waypoint NorthPole;
      extern const Waypoint SouthPole;
      extern const Waypoint EquatorialMeridian;
      extern const Waypoint EquatorialAntiMeridian;
      extern const Waypoint CliftonCampus;
      extern const Waypoint CityCampus;
      extern const Waypoint Pontianak;

      extern const metres meanRadius;
      extern const metres equatorialCircumference;
      extern const metres polarCircumference;


      /* Determine the east/west circumference of the Earth at a specified latitude.
       *
       * Pre-condition: The latitude is a valid latitude angle.
       */
      metres circumferenceAtLatitude(degrees lat);


      /* Determine the change in degrees latitude from a North/South distance (in metres).
       *
       * Pre-condition: The distance is no greater than the Earth's polar circumference.
       */
      degrees latitudeSubtendedBy(metres northSouthDistance);


      /* Determine the change in degrees longitude from an East/West distance (in metres).
       * Unlike North/South changes, the change in degrees longitude also depends on the
       * latitude where the East/West line occurs.
       *
       * Pre-condition: The latitude is a valid latitude angle.
       *
       * Pre-condition: The distance is no greater than the Earth's circumference at the
       * specified latitude.
       */
      degrees longitudeSubtendedBy(metres eastWestDistance, degrees lat);


      /* Check that the altitude value is no lower than the centre of the Earth.
       * No upper limit is imposed.
       */
      bool isValidAltitude(metres);

  }
}

#endif

