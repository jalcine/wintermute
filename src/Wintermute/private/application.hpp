/**
 * Copyright (C) 2013 Jacky Alcine <me@jalcine.me>
 *
 * This file is part of Wintermute, the extensible AI platform.
 *
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
#include "Wintermute/private/Procedure/module.hpp"
#include "Wintermute/Procedure/process_module.hpp"
#include <QtCore/QSharedPointer>
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtZeroMQ/globals.hpp>

namespace Wintermute
{
class ApplicationPrivate
{
  friend class Wintermute::Procedure::ModulePrivate;

public:
  QSharedPointer<QCoreApplication> app;
  QSharedPointer<Procedure::ProcessModule> module;
  QList<Procedure::Module*> modules;
  QSettings* settings;

  ApplicationPrivate ( int& argc, char** argv ) :
    app(), module(), modules(), settings ( 0 ) {
    app = QSharedPointer<QCoreApplication> ( new QCoreApplication ( argc, argv ) );
    module.clear();
  }

  void initialize() {
    // Add library paths for plug-ins.
    // TODO: Add more potential paths for plugins.
    // TODO: Allow paths to be specified over command-line and environment.
    app->addLibraryPath ( WINTERMUTE_PLUGIN_LIBRARY_DIR );
    // Define context for platform.
    Procedure::ModulePrivate::context = QtZeroMQ::createDefaultContext ( Wintermute::Application::instance() );
    // Allocate necessary variables for logging and arguments.
    // TODO: Move factory initialization to separate thread.
    Logging::instance();
    Arguments::instance();
    Factory::instance();
  }

  int exec() {
    return app->exec();
  }

  void loadProcessModule() {
    Procedure::ProcessModule* modulePtr = new Procedure::ProcessModule;
    module = QSharedPointer<Procedure::ProcessModule> ( modulePtr );
  }

  void loadCurrentMode() {
    Factory::instance()->start();
    const QString mode = Arguments::instance()->argument ( "mode" ).toString();
    if ( mode == "daemon" || mode == "d" ) {
      bool heartBeatLoaded = Factory::instance()->loadPlugin ( "wintermute-daemon" );
      if ( !heartBeatLoaded ) {
        werr ( Application::instance(), "Can't load daemon plugin; bailing out!" );
        Application::instance()->stop ( 127 );
      }
    } else if ( mode == "plugin" || mode == "p" ) {
      wdebug ( Application::instance(), "Booting plugin..." );
      const QString pluginName ( Arguments::instance()->argument ( "plugin" ).toString() );
      if ( pluginName.isNull() ) {
        werr ( Application::instance(), "Invalid plugin name provided." );
        Application::instance()->stop ( 127 );
        return;
      }
      Factory::instance()->loadPlugin ( pluginName );
    }
  }
};
}
