#include <cassert>
#include <cmath>
#include <stdexcept>

#include "geometry.h"
#include "earth.h"
#include "waypoint.h"

namespace GPS
{
  Waypoint::Waypoint(degrees lat, degrees lon, metres alt)
  {
      if (! isValidLatitude(lat))
          throw std::invalid_argument("Latitude values must not exceed " + std::to_string(poleLatitude) + " degrees.");

      if (! isValidLongitude(lon))
          throw std::invalid_argument("Longitude values must not exceed " + std::to_string(antiMeridianLongitude) + " degrees.");

      if (! Earth::isValidAltitude(alt))
          throw std::invalid_argument("Negative altitude values must not exceed the Earth's mean radius (i.e. must not be below the centre of the Earth).");

      _latitude = lat;
      _longitude = lon;
      _altitude = alt;
  }

  degrees Waypoint::latitude() const
  {
      return _latitude;
  }

  degrees Waypoint::longitude() const
  {
      return _longitude;
  }

  metres Waypoint::altitude() const
  {
      return _altitude;
  }

  metres Waypoint::horizontalDistanceBetween(Waypoint p1, Waypoint p2)
  /* The Haversine Formula
   * See: https://en.wikipedia.org/wiki/Haversine_formula
   */
  {
      const radians lat1 = degToRad(p1.latitude());
      const radians lat2 = degToRad(p2.latitude());
      const radians lon1 = degToRad(p1.longitude());
      const radians lon2 = degToRad(p2.longitude());

      const double h = sinSqr((lat2-lat1)/2) + std::cos(lat1)*std::cos(lat2)*sinSqr((lon2-lon1)/2);
      return 2 * Earth::meanRadius * std::asin(std::sqrt(h));
  }

  metres Waypoint::verticalDistanceBetween(Waypoint p1, Waypoint p2)
  {
      return std::abs(p2.altitude() - p1.altitude());
  }
}
