/*****************************************************************************
* Copyright 2015-2018 Alexander Barthel albar965@mailbox.org
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

#include "route/routealtitudeleg.h"

float RouteAltitudeLeg::getMaxAltitude() const
{
  if(geometry.isEmpty())
    return map::INVALID_ALTITUDE_VALUE;
  else
    // Rectangle bottom and top are revered since this is no screen coordinate system
    return static_cast<float>(geometry.boundingRect().bottom());
}

float RouteAltitudeLeg::getDistanceFromStart() const
{
  if(geometry.isEmpty())
    return map::INVALID_DISTANCE_VALUE;
  else
    return static_cast<float>(geometry.last().x());
}

float RouteAltitudeLeg::getDistanceTo() const
{
  if(geometry.isEmpty())
    return map::INVALID_DISTANCE_VALUE;
  else
    return static_cast<float>(getDistanceFromStart() - geometry.first().x());
}

void RouteAltitudeLeg::setAlt(float alt)
{
  // Set altitude for all points
  for(QPointF& pt : geometry)
    pt.setY(alt);
}
