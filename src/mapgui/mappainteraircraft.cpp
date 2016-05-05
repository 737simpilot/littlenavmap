/*****************************************************************************
* Copyright 2015-2016 Alexander Barthel albar965@mailbox.org
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

#include "mappainteraircraft.h"
#include "mapwidget.h"
#include "mapscale.h"
#include "mapgui/mapquery.h"
#include "common/mapcolors.h"
#include "geo/calculations.h"
#include "mapgui/symbolpainter.h"

#include <algorithm>
#include <marble/GeoDataLineString.h>
#include <marble/GeoPainter.h>
#include <marble/MarbleWidget.h>
#include <marble/ViewportParams.h>

using namespace Marble;
using namespace atools::geo;
using namespace maptypes;

const QVector<QLine> AIRCRAFTLINES({QLine(0, -20, 0, 16),
                                    QLine(-20, 2, 0, -6), QLine(0, -6, 20, 2),
                                    QLine(-10, 18, 0, 14), QLine(0, 14, 10, 18)});

MapPainterAircraft::MapPainterAircraft(MapWidget *widget, MapQuery *mapQuery, MapScale *mapScale,
                                       bool verboseMsg)
  : MapPainter(widget, mapQuery, mapScale, verboseMsg), navMapWidget(widget)
{
}

MapPainterAircraft::~MapPainterAircraft()
{

}

void MapPainterAircraft::render(const PaintContext *context)
{
  if(!context->objectTypes.testFlag(AIRCRAFT))
    return;

  setRenderHints(context->painter);

  context->painter->save();
  paintAircraft(context->painter);
  context->painter->restore();
}

void MapPainterAircraft::paintAircraft(GeoPainter *painter)
{
  const Pos& pos = navMapWidget->getSimConnectData().getPosition();

  if(!pos.isValid())
    return;

  int x, y;
  if(wToS(pos, x, y))
  {
    painter->translate(x, y);
    painter->rotate(atools::geo::normalizeCourse(navMapWidget->getSimConnectData().getCourseTrue()));

    painter->setPen(mapcolors::aircraftBackPen);
    painter->drawLines(AIRCRAFTLINES);

    painter->setPen(mapcolors::aircraftFillPen);
    painter->drawLines(AIRCRAFTLINES);

    painter->resetTransform();
  }
}
