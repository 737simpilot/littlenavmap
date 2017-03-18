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

#ifndef LITTLENAVMAP_PROCTREECONTROLLER_H
#define LITTLENAVMAP_PROCTREECONTROLLER_H

#include "common/maptypes.h"
#include "search/abstractsearch.h"

#include <QBitArray>
#include <QFont>
#include <QObject>
#include <QVector>

namespace atools {
namespace gui {
class ItemViewZoomHandler;
class GridDelegate;
}

namespace sql {
class SqlRecord;
}
namespace geo {
class Pos;
class Rect;
}
}

class InfoQuery;
class QTreeWidget;
class QTreeWidgetItem;
class MainWindow;
class ProcedureQuery;
class HtmlInfoBuilder;
class TreeEventFilter;

/* Takes care of the tree widget in approach tab on the informtaion window. */
class ProcedureSearch :
  public AbstractSearch
{
  Q_OBJECT

public:
  ProcedureSearch(MainWindow *main, QTreeWidget *treeWidgetParam, int tabWidgetIndex);
  virtual ~ProcedureSearch();

  /* Fill tree widget and index with all approaches and transitions of an airport */
  void showProcedures(maptypes::MapAirport airport);

  /* Save tree view state */
  virtual void saveState() override;
  virtual void restoreState() override;

  /* Update fonts units, etc. */
  virtual void optionsChanged() override;

  virtual void preDatabaseLoad() override;
  virtual void postDatabaseLoad() override;

  /* No op overrides */
  virtual void getSelectedMapObjects(maptypes::MapSearchResult& result) const override;
  virtual void connectSearchSlots() override;
  virtual void updateUnits() override;
  virtual void updateTableSelection() override;

signals:
  /* Show approaches and highlight circles on the map */
  void procedureSelected(maptypes::MapProcedureRef);
  void procedureLegSelected(maptypes::MapProcedureRef);

  /* Zoom to approaches/transitions or waypoints */
  void showPos(const atools::geo::Pos& pos, float zoom, bool doubleClick);
  void showRect(const atools::geo::Rect& rect, bool doubleClick);

  /* Add the complete procedure to the route */
  void routeInsertProcedure(const maptypes::MapProcedureLegs& legs);

  /* Show information info window on navaid on double click */
  void showInformation(maptypes::MapSearchResult result);

private:
  enum FilterIndex
  {
    FILTER_ALL_PROCEDURES,
    FILTER_DEPARTURE_PROCEDURES,
    FILTER_ARRIVAL_PROCEDURES,
    FILTER_APPROACH_AND_TRANSITIONS
  };

  void itemSelectionChanged();
  void itemDoubleClicked(QTreeWidgetItem *item, int column);

  /* Load legs dynamically as approaches or transitions are expanded */
  void itemExpanded(QTreeWidgetItem *item);

  void contextMenu(const QPoint& pos);

  // Save and restore expanded and selected item state
  QBitArray saveTreeViewState();
  void restoreTreeViewState(const QBitArray& state);

  /* Build full approach or transition items for the tree view */
  QTreeWidgetItem *buildApproachItem(QTreeWidgetItem *runwayItem, const atools::sql::SqlRecord& recApp);
  QTreeWidgetItem *buildTransitionItem(QTreeWidgetItem *apprItem, const atools::sql::SqlRecord& recTrans);

  /* Build an leg for the selected/table or tree view */
  void buildLegItem(QTreeWidgetItem *parentItem, const maptypes::MapProcedureLeg& leg);

  /* Highlight missing navaids red */
  void setItemStyle(QTreeWidgetItem *item, const maptypes::MapProcedureLeg& leg);

  /* Show transition, approach or waypoint on map */
  void showEntry(QTreeWidgetItem *item, bool doubleClick);

  /* Update course and distances in the approach legs when a preceding transition is selected */
  void updateApproachItem(QTreeWidgetItem *apprItem, int transitionId);

  void addApproachLegs(const maptypes::MapProcedureLegs *legs, QTreeWidgetItem *item, int transitionId);
  void addTransitionLegs(const maptypes::MapProcedureLegs *legs, QTreeWidgetItem *item);
  void fillApproachTreeWidget();

  /* Fill header for tree or selected/table view */
  void updateTreeHeader();
  void createFonts();

  QTreeWidgetItem *parentApproachItem(QTreeWidgetItem *item) const;
  QTreeWidgetItem *parentTransitionItem(QTreeWidgetItem *item) const;

  maptypes::MapObjectTypes buildType(const atools::sql::SqlRecord& recApp);
  void updateHeaderLabel();
  void filterIndexChanged(int index);
  void filterIndexRunwayChanged(int index);
  void clearRunwayFilter();
  void fillRunwayFilter();
  void resetSearch();

  // item's types are the indexes into this array with approach, transition and leg ids
  QVector<maptypes::MapProcedureRef> itemIndex;

  // Item type is the index into this array
  // Approach or transition legs are already loaded in tree if bit is set
  // Fist bit in pair: expanded or not, Second bit: selection state
  QBitArray itemLoadedIndex;

  InfoQuery *infoQuery = nullptr;
  ProcedureQuery *procedureQuery = nullptr;
  QTreeWidget *treeWidget = nullptr;
  MainWindow *mainWindow = nullptr;
  QFont transitionFont, approachFont, legFont, missedLegFont, invalidLegFont, activeLegFont, identFont;
  maptypes::MapAirport currentAirport;

  // Maps airport ID to expanded state of the tree widget items - bit array is same content as itemLoadedIndex
  QHash<int, QBitArray> recentTreeState;

  /* Used to make the table rows smaller and also used to adjust font size */
  atools::gui::ItemViewZoomHandler *zoomHandler = nullptr;
  atools::gui::GridDelegate *gridDelegate = nullptr;

  FilterIndex filterIndex = FILTER_ALL_PROCEDURES;
  TreeEventFilter *treeEventFilter = nullptr;

};

#endif // LITTLENAVMAP_PROCTREECONTROLLER_H