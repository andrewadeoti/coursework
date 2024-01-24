#include <cmath>
#include <cassert>

#include "geometry.h"
#include "waypoint.h"
#include "earth.h"

namespace GPS
{
  namespace Earth
  {
      const Waypoint NorthPole = Waypoint(poleLatitude,0,0);
      const Waypoint SouthPole = Waypoint(-poleLatitude,0,0);
      const Waypoint EquatorialMeridian = Waypoint(0,0,0);
      const Waypoint EquatorialAntiMeridian = Waypoint(0,antiMeridianLongitude,0);
      const Waypoint CliftonCampus = Waypoint(52.91249953,-1.18402513,58);
      const Waypoint CityCampus = Waypoint(52.9581383,-1.1542364,53);
      const Waypoint Pontianak = Waypoint(0,109.322134,0);

      const metres meanRadius = 6371008.8;
      const metres equatorialCircumference = 40075160;
      const metres polarCircumference = 40008000;

      metres circumferenceAtLatitude(degrees lat)
      {
          assert (isValidLatitude(lat));

          return equatorialCircumference * std::cos(degToRad(lat));
      }

      degrees latitudeSubtendedBy(metres northSouthDistance)
      {
          assert (! (northSouthDistance > polarCircumference));

          return (northSouthDistance / polarCircumference) * fullRotation;
      }

      degrees longitudeSubtendedBy(metres eastWestDistance, degrees lat)
      {
          assert (isValidLatitude(lat));

          metres circumferenceAtThisLatitude = circumferenceAtLatitude(lat);

          assert (eastWestDistance <= circumferenceAtThisLatitude);

          if (circumferenceAtThisLatitude == 0)
          {
              return 0; // No longitude at poles.
          }
          else
          {
              return (eastWestDistance / circumferenceAtThisLatitude) * fullRotation;
          }
      }

      bool isValidAltitude(metres altitude)
      {
          return altitude >= -meanRadius;
      }

  }
}
