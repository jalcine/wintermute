/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2013 Jacky Alciné <me@jalcine.me>
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
#include "Wintermute/Events/Filters/call.hpp"
#include "Wintermute/Procedure/dummy_dispatcher.hpp"
#include "Wintermute/Procedure/dummy_receiver.hpp"
#include "Wintermute/Procedure/process_module.hpp"
#include <QtCore/QSharedPointer>
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QDir>

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
  Wintermute::Application* q_ptr;

  ApplicationPrivate ( int& argc, char** argv, Wintermute::Application* q ) :
    app(), module(), modules(), settings ( 0 ), q_ptr ( q ) {
    app = QSharedPointer<QCoreApplication> ( new QCoreApplication ( argc, argv ) );
    module.clear();
    modules.clear();
    this->installEventFilters();
  }

  void installEventFilters() {
    Events::Filters::CallFilter* callFilter = new Events::Filters::CallFilter();
    callFilter->setParent ( q_ptr );
    app->installEventFilter ( callFilter );
  }

  void addLibraryPaths() {
    app->addLibraryPath ( WINTERMUTE_PLUGIN_LIBRARY_DIR );
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if ( env.contains ( "WINTERMUTE_PLUGIN_LIBRARY_DIR" ) ) {
      QStringList envLibDirs = env.value ( "WINTERMUTE_PLUGIN_LIBRARY_DIR" ).split ( ":", QString::SkipEmptyParts );
      Q_FOREACH ( const QString libDir, envLibDirs ) {
        app->addLibraryPath ( libDir );
      }
    }
  }

  void initialize() {
    this->addLibraryPaths();
    Logging::instance();
    Arguments::instance();
    Factory::instance();
  }

  int exec() {
    return app->exec();
  }

  void loadProcessModule() {
    Procedure::DummyDispatcher* dummyDsptcher = new Procedure::DummyDispatcher;
    Procedure::DummyReceiver* dummyRcvr = new Procedure::DummyReceiver;
    dummyDsptcher->setParent ( app.data() );
    dummyRcvr->setParent ( app.data() );
    module = QSharedPointer<Procedure::ProcessModule> ( new Procedure::ProcessModule );
  }

  void loadCurrentMode() {
    const QString mode = Arguments::instance()->argument ( "mode" ).toString();
    if ( mode == "daemon" || mode == "d" ) {
      bool daemonLoaded = Factory::instance()->loadPlugin ( "wintermute-daemon" );
      if ( !daemonLoaded ) {
        werr ( Application::instance(), "Can't load daemon plugin; bailing out!" );
        Application::instance()->stop ( 127 );
      }
    } else if ( mode == "plugin" || mode == "p" ) {
      wdebug ( Application::instance(), "Booting plugin..." );
      const QString pluginName ( Arguments::instance()->argument ( "plugin" ).toString() );
      Factory::instance()->loadPlugin ( pluginName );
    }
  }
};
}
