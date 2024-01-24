#ifndef GPS_WAYPOINT_H
#define GPS_WAYPOINT_H

#include <string>

#include "types.h"

namespace GPS
{
  /* A Waypoint object represents a location on, within, or above, the Earth's surface.
   * The location is identifed by degrees latitude, degrees longitude, and altitude.
   * Altitude is relative to the Earth's mean radius, i.e. roughly sea-level.
   */
  class Waypoint
  {
    public:
      /* Construct a Waypoint from degrees latitude, degrees longitude, and altitude in metres.
       *
       * Throws a std::invalid_argument exception for:
       *   - geometrically invalid latitude or longitude values,
       *   - negative altitude values that exceed the Earth's mean radius,
       *     i.e. those that would be below the centre of the Earth.
       */
      Waypoint(degrees lat, degrees lon, metres alt);

      degrees latitude() const;
      degrees longitude() const;
      metres  altitude() const;

      /* Computes an approximation of the horizontal distance between two Waypoints on the Earth's surface.
       * Note that this does NOT take into account altitude.
       */
      static metres horizontalDistanceBetween(Waypoint, Waypoint);

      /* Computes an approximation of the vertical distance between two Waypoints,
       * i.e. the difference in elevations.  This always returns a positive value.
       */
      static metres verticalDistanceBetween(Waypoint, Waypoint);

    private:
      degrees _latitude;
      degrees _longitude;
      metres  _altitude;
  };
}

#endif
