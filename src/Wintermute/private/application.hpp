/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 *
 * Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "Wintermute/logging.hpp"
#include "Wintermute/arguments.hpp"
#include "Wintermute/factory.hpp"
#include "Wintermute/Events/call_filter.hpp"
#include "Wintermute/Procedure/process_module.hpp"
#include <QtCore/QSharedPointer>
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QDir>
#include <iostream>

namespace Wintermute
{
  class Application;
  class ApplicationPrivate
  {

    public:
      QScopedPointer<QCoreApplication> app;
      QScopedPointer<QSettings> settings;
      QPointer<Application> q_ptr;
      QPointer<Procedure::ProcessModule> module;

      ApplicationPrivate ( int &argc, char **argv, Application *q ) :
        app(), settings ( nullptr ), q_ptr ( nullptr ), module ( nullptr ) {
        app.reset(new QCoreApplication ( argc, argv ));
      }

      void installEventFilters() {
        winfo ( q_ptr, "Adding event filters...");
        Events::CallFilter *callFilter = new Events::CallFilter();
        callFilter->setParent ( q_ptr );
        app->installEventFilter ( callFilter );
        winfo ( q_ptr, "Added event filters.");
      }

      void addLibraryPaths() {
        winfo ( q_ptr, "Updating library search paths...");
        app->addLibraryPath ( WINTERMUTE_PLUGIN_LIBRARY_DIR );
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        if ( env.contains ( "WINTERMUTE_PLUGIN_LIBRARY_DIR" ) ) {
          winfo(app.data(),
                "Pulling library directories from $WINTERMUTE_PLUGIN_LIBRARY_DIR...");
          QStringList envLibDirs = env.value ( "WINTERMUTE_PLUGIN_LIBRARY_DIR" ).
                                   split ( ":", QString::SkipEmptyParts );
          Q_FOREACH ( const QString libDir, envLibDirs ) {
            app->addLibraryPath ( libDir );
            winfo(app.data(),
                  QString("Added %1 to the library discovery path.").arg(libDir));
          }
        } else {
          winfo(app.data(),
                "No library directories to read from $WINTERMUTE_PLUGIN_LIBRARY_DIR.");
        }
        winfo ( q_ptr, "Updated library search paths.");
      }

      void initialize() {
        addLibraryPaths();
        Logging::instance();
        Arguments::instance();
        Factory::instance();
      }

      int exec() {
        int returnCode = 0x0;
        try {
          returnCode = app->exec();
        } catch (...) {
          werr(app.data(), "An fatal error occurred in Wintermute.");
          returnCode = 0xff;
        }
        return returnCode;
      }

      void loadProcessModule() {
        module = new Procedure::ProcessModule;
      }

      void loadCurrentMode() {
        const QString mode = Arguments::instance()->argument ( "mode" ).toString();
        if ( mode == "daemon" || mode == "d" ) {
          loadDaemonState();
        } else if ( mode == "plugin" || mode == "p") {
          loadPluginState();
        } else {
          werr ( Application::instance(), "Plugin can't be determined for loading." );
          Application::instance()->stop ( WINTERMUTE_ERROR_MODE_FAILED );
        }
      }

      void loadDaemonState() {
        if ( !wntrFactory->loadPlugin ( "wintermute-daemon" ) ) {
          werr ( wntrApp, "Can't load daemon plugin; bailing out!" );
          Application::instance()->stop ( WINTERMUTE_ERROR_MODE_FAILED );
        } else {
          winfo ( wntrApp, "Daemon started." );
        }
      }

      void loadPluginState() {
        const QString pluginName ( Arguments::instance()->argument ( "plugin" )
                                   .toString() );
        if ( ! ( pluginName.isEmpty() && pluginName.isNull() ) ) {
          wdebug ( Application::instance(), QString("Booting plugin '%1'...")
                   .arg( pluginName ) );
          if ( !Factory::instance()->loadPlugin ( pluginName ) ) {
            werr ( wntrApp, QString("Failed to load plugin %1.").arg(pluginName) );
            Application::instance()->stop ( WINTERMUTE_ERROR_MODE_FAILED );
          } else {
            winfo ( wntrApp, QString("Booted plugin %1 for plug-in mode.")
                    .arg(pluginName));
          }
        }
      }
  };
}
