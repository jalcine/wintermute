/**
 * vim: ft=qt.cpp
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
#include <QtZeroMQ/globals.hpp>

#define WINTERMUTE_PLUGIN_MODULE_HEARTBEAT "me.jalcine.wintermute.heartbeat"

namespace Wintermute {
  class ApplicationPrivate {
    friend class Wintermute::Procedure::ModulePrivate;

    public:
    QSharedPointer<QCoreApplication> app;
    QSharedPointer<Procedure::ProcessModule> module;
    QList<Procedure::Module*> modules;
    QSettings* settings;

    ApplicationPrivate(int &argc, char **argv) : settings(0) {
      app = QSharedPointer<QCoreApplication>(new QCoreApplication(argc,argv));
      module.clear();
    }

    void initialize(){
      // Add library paths for plug-ins.
      app->addLibraryPath(WINTERMUTE_PLUGIN_LIBRARY_DIR);

      // Define context for platform.
      Procedure::ModulePrivate::context = QtZeroMQ::createDefaultContext(Wintermute::Application::instance());

      // Allocate necessary variables for logging and arguments.
      // TODO: Move factory initialization to separate thread.
      Logging::instance();
      Arguments::instance();
      Factory::instance();
    }

    int exec(){
      return app->exec();
    }

    void loadProcessModule() {
      // Create the module.
      Procedure::ProcessModule* modulePtr = new Procedure::ProcessModule;
      module = QSharedPointer<Procedure::ProcessModule>(modulePtr);

      // TODO: Say 'hello!'.
      module->invoke("hello", QVariantList());
    }

    void loadCurrentMode() {
      // Load each instance.
      const QString mode = Arguments::instance()->argument("mode").toString();

      if (mode == "daemon" || mode == "d"){
        wdebug(Application::instance(), "Starting daemon mode...");
        Factory::instance()->start();

        if (!Arguments::instance()->hasArgument("fork")){
          // TODO: Fork this process.
          // TODO: Exit this process.
        }
      } else {
        Factory::instance()->loadPlugin(WINTERMUTE_PLUGIN_MODULE_HEARTBEAT);
      }
    }
  };
}
