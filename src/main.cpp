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

#include "gui/mainwindow.h"
#include "logging/loggingdefs.h"
#include "logging/logginghandler.h"
#include "logging/loggingutil.h"
#include "settings/settings.h"
#include "gui/translator.h"
#include "exception.h"
#include "gui/errorhandler.h"

#include <QSettings>
#include <QApplication>

int main(int argc, char *argv[])
{
  // Initialize the resources from atools static library
  Q_INIT_RESOURCE(atools);

  int retval = 0;
  QApplication app(argc, argv);
  QApplication::setWindowIcon(QIcon(":/littlenavmap/resources/icons/navroute.svg"));
  QCoreApplication::setApplicationName("Little Navmap");
  QCoreApplication::setOrganizationName("ABarthel");
  QCoreApplication::setOrganizationDomain("abarthel.org");
  QCoreApplication::setApplicationVersion("1.5.0.develop");

  try
  {
    using atools::logging::LoggingHandler;
    using atools::logging::LoggingUtil;
    using atools::settings::Settings;
    using atools::gui::Translator;

    // Initialize logging and force logfiles into the system or user temp directory
    LoggingHandler::initializeForTemp(atools::settings::Settings::getOverloadedPath(
                                        ":/littlenavmap/resources/config/logging.cfg"));

    // Print some information which can be useful for debugging
    LoggingUtil::logSystemInformation();
    LoggingUtil::logStandardPaths();
    Settings::logSettingsInformation();

    Settings& s = Settings::instance();
    // Load local and Qt system translations from various places
    Translator::load(s->value("Options/Language", QString()).toString());

    // Write version to configuration file
    QString oldVersion = s->value("Options/Language").toString();
    qInfo() << "Found version" << oldVersion << "in configuration file";
    s.getAndStoreValue("Options/Version", QCoreApplication::applicationVersion());
    s.syncSettings();

    MainWindow mainWindow;
    mainWindow.show();

    qDebug() << "Before app.exec()";
    retval = app.exec();

    qDebug() << "app.exec() done, retval is" << retval;
  }
  catch(atools::Exception& e)
  {
    atools::gui::ErrorHandler(nullptr).handleException(e);
    retval = 1;
  }
  catch(...)
  {
    atools::gui::ErrorHandler(nullptr).handleUnknownException();
    retval = 1;
  }
  return retval;

}