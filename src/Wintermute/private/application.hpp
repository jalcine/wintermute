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

#include <QtCore/QSharedPointer>
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include "logging.hpp"
#include "arguments.hpp"
#include "factory.hpp"

namespace Wintermute {
  class ApplicationPrivate {
    public:
      QSharedPointer<QCoreApplication> app;
      QSharedPointer<Procedure::Module> module;
      QSettings* settings;

      ApplicationPrivate(int &argc, char **argv) : settings(0) {
        app = QSharedPointer<QCoreApplication>(new QCoreApplication(argc,argv));
        module.clearData();
      }

      void initialize(){
        // Add library paths for plug-ins.
        app->addLibraryPath(WINTERMUTE_PLUGIN_LIBRARY_DIR);

        // Allocate necessary variables for logging and arguments.
        // TODO: Move factory initialization to separate thread.
        Logging::instance();
        Arguments::instance();
        Factory::instance();
      }

      int exec(){
        return app->exec();
      }
  };

}
