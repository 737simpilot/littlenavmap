/*****************************************************************************
* Copyright 2015-2017 Alexander Barthel albar965@mailbox.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef LITTLENAVMAP_MAPFLAGS_H
#define LITTLENAVMAP_MAPFLAGS_H

#include <QString>

/*
 * Maptypes are mostly filled from database tables and are used to pass airport, navaid and more information
 * around in the program. The types are kept primitive (no inheritance no vtable) for performance reasons.
 * Units are usually feet. Type string are as they appear in the database.
 */
namespace map {

/* Value for invalid/not found distances */
Q_DECL_CONSTEXPR static float INVALID_COURSE_VALUE = std::numeric_limits<float>::max();
Q_DECL_CONSTEXPR static float INVALID_DISTANCE_VALUE = std::numeric_limits<float>::max();
Q_DECL_CONSTEXPR static int INVALID_INDEX_VALUE = std::numeric_limits<int>::max();

Q_DECL_CONSTEXPR static float INVALID_MAGVAR = 9999.f;

/* Type covering all objects that are passed around in the program. Also use to determine what should be drawn. */
enum MapObjectType
{
  NONE = 0,
  AIRPORT = 1 << 0,
  AIRPORT_HARD = 1 << 1,
  AIRPORT_SOFT = 1 << 2,
  AIRPORT_EMPTY = 1 << 3,
  AIRPORT_ADDON = 1 << 4,
  VOR = 1 << 5,
  NDB = 1 << 6,
  ILS = 1 << 7,
  MARKER = 1 << 8,
  WAYPOINT = 1 << 9,
  AIRWAY = 1 << 10,
  AIRWAYV = 1 << 11,
  AIRWAYJ = 1 << 12,
  FLIGHTPLAN = 1 << 13, /* Flight plan */
  AIRCRAFT = 1 << 14, /* Simulator aircraft */
  AIRCRAFT_AI = 1 << 15, /* AI or multiplayer Simulator aircraft */
  AIRCRAFT_TRACK = 1 << 16, /* Simulator aircraft track */
  USER = 1 << 17, /* Flight plan user waypoint */
  PARKING = 1 << 18,
  RUNWAYEND = 1 << 19,
  INVALID = 1 << 20, /* Flight plan waypoint not found in database */
  MISSED_APPROACH = 1 << 21, /* Only procedure type that can be hidden */
  PROCEDURE = 1 << 22, /* General procedure leg */
  AIRSPACE = 1 << 23, /* General airspace boundary */

  AIRPORT_ALL = AIRPORT | AIRPORT_HARD | AIRPORT_SOFT | AIRPORT_EMPTY | AIRPORT_ADDON,
  NAV_ALL = VOR | NDB | WAYPOINT,
  NAV_MAGVAR = AIRPORT | VOR | NDB | WAYPOINT, /* All objects that have a magvar assigned */

  ALL = 0xffffffff
};

Q_DECLARE_FLAGS(MapObjectTypes, MapObjectType);
Q_DECLARE_OPERATORS_FOR_FLAGS(map::MapObjectTypes);

QDebug operator<<(QDebug out, const map::MapObjectTypes& type);

enum MapAirspaceType
{
  AIRSPACE_NONE = 0,
  CENTER = 1 << 0,
  CLASS_A = 1 << 1, // ICAO airspace - controlled - no VFR
  CLASS_B = 1 << 2, // ICAO airspace - controlled
  CLASS_C = 1 << 3, // ICAO airspace - controlled
  CLASS_D = 1 << 4, // ICAO airspace - controlled
  CLASS_E = 1 << 5, // ICAO airspace - controlled
  CLASS_F = 1 << 6, // Open FIR - uncontrolled
  CLASS_G = 1 << 7, // Open FIR - uncontrolled
  TOWER = 1 << 8,
  CLEARANCE = 1 << 9,
  GROUND = 1 << 10,
  DEPARTURE = 1 << 11,
  APPROACH = 1 << 12,
  MOA = 1 << 13,
  RESTRICTED = 1 << 14,
  PROHIBITED = 1 << 15,
  WARNING = 1 << 16,
  ALERT = 1 << 17,
  DANGER = 1 << 18,
  NATIONAL_PARK = 1 << 19,
  MODEC = 1 << 20,
  RADAR = 1 << 21,
  TRAINING = 1 << 22,

  /* Special filter flags - not airspaces */
  AIRSPACE_BELOW_10000 = 1 << 23,
  AIRSPACE_BELOW_18000 = 1 << 24,
  AIRSPACE_ABOVE_10000 = 1 << 25,
  AIRSPACE_ABOVE_18000 = 1 << 26,
  AIRSPACE_AT_FLIGHTPLAN = 1 << 27,
  AIRSPACE_ALL_ALTITUDE = 1 << 28,

  /* Action flags - not airspaces */
  AIRSPACE_ALL_ON = 1 << 29,
  AIRSPACE_ALL_OFF = 1 << 30,

  AIRSPACE_ICAO = CLASS_A | CLASS_B | CLASS_C | CLASS_D | CLASS_E,
  AIRSPACE_FIR = CLASS_F | CLASS_G,
  AIRSPACE_CENTER = CENTER,
  AIRSPACE_RESTRICTED = RESTRICTED | PROHIBITED | MOA | DANGER,
  AIRSPACE_SPECIAL = WARNING | ALERT | TRAINING,
  AIRSPACE_OTHER = TOWER | CLEARANCE | GROUND | DEPARTURE | APPROACH | MODEC | RADAR | NATIONAL_PARK,

  AIRSPACE_FOR_VFR = CLASS_B | CLASS_C | CLASS_D | CLASS_E | AIRSPACE_FIR,
  AIRSPACE_FOR_IFR = CLASS_A | CLASS_B | CLASS_C | CLASS_D | CLASS_E | CENTER,

  AIRSPACE_ALL = AIRSPACE_ICAO | AIRSPACE_FIR | AIRSPACE_CENTER | AIRSPACE_RESTRICTED | AIRSPACE_SPECIAL |
                 AIRSPACE_OTHER,

  // Default value on first start
  AIRSPACE_DEFAULT = AIRSPACE_ICAO | AIRSPACE_RESTRICTED | AIRSPACE_ALL_ALTITUDE
};

Q_DECL_CONSTEXPR int MAP_AIRSPACE_TYPE_BITS = 22;

/* Airport flags coverting most airport attributes and facilities. */
enum MapAirportFlag
{
  AP_NONE = 0,
  AP_ADDON = 1 << 0,
  AP_LIGHT = 1 << 1, /* Has at least one lighted runway */
  AP_TOWER = 1 << 2, /* Has a tower frequency */
  AP_ILS = 1 << 3, /* At least one runway end has ILS */
  AP_PROCEDURE = 1 << 4, /* At least one runway end has an approach */
  AP_MIL = 1 << 5,
  AP_CLOSED = 1 << 6, /* All runways are closed */
  AP_AVGAS = 1 << 7,
  AP_JETFUEL = 1 << 8,
  AP_HARD = 1 << 9, /* Has at least one hard runway */
  AP_SOFT = 1 << 10, /* Has at least one soft runway */
  AP_WATER = 1 << 11, /* Has at least one water runway */
  AP_HELIPAD = 1 << 12,
  AP_APRON = 1 << 13,
  AP_TAXIWAY = 1 << 14,
  AP_TOWER_OBJ = 1 << 15,
  AP_PARKING = 1 << 16,
  AP_ALS = 1 << 17, /* Has at least one runway with an approach lighting system */
  AP_VASI = 1 << 18, /* Has at least one runway with a VASI */
  AP_FENCE = 1 << 19,
  AP_RW_CLOSED = 1 << 20, /* Has at least one closed runway */
  AP_COMPLETE = 1 << 21, /* Struct completely loaded? */
  AP_ALL = 0xffffffff
};

Q_DECLARE_FLAGS(MapAirportFlags, MapAirportFlag);
Q_DECLARE_OPERATORS_FOR_FLAGS(map::MapAirportFlags);

Q_DECLARE_FLAGS(MapAirspaceTypes, MapAirspaceType);
Q_DECLARE_OPERATORS_FOR_FLAGS(map::MapAirspaceTypes);

} // namespace types

#endif // LITTLENAVMAP_MAPFLAGS_H