/**
 *
 * Copyright (C) 2013 Jacky Alcine <jacky.alcine@thesii.org>
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

#include "application.hpp"
#include "arguments.hpp"
#include "logging.hpp"
#include "factory.hpp"
#include "version.hpp"
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QSharedPointer>

using Wintermute::Arguments;
using Wintermute::Logging;
using Wintermute::Version;
using Wintermute::Factory;
using Wintermute::Application;
using Wintermute::ApplicationPrivate;

namespace Wintermute {
  class ApplicationPrivate {
    public:
      QSharedPointer<QCoreApplication> app;

      ApplicationPrivate(int &argc, char **argv) {
        app = QSharedPointer<QCoreApplication>(new QCoreApplication(argc,argv));
      }

      void
      initialize(){
          // Allocate necessary variables for logging and arguments.
          Logging::instance();
          Arguments::instance();
          Factory::instance();

          // Add library paths for plug-ins.
          app->addLibraryPath(WINTERMUTE_PLUGIN_LIBRARY_DIR);
      }

      int
      exec(){
          return app->exec();
      }
  };

}

Application* Application::self = 0;

Application::Application(int &argc, char **argv) : QObject(), d_ptr(new ApplicationPrivate(argc,argv)){
  Q_D(Application);

  // I am self!
  Application::self = qobject_cast<Application*>(this);

  // Define the application in Qt.
  d->app->setApplicationName("Wintermute");
  d->app->setApplicationVersion(this->version().toString());
  d->app->setOrganizationName("Synthetic Intellect Institute");
  d->app->setOrganizationDomain("thesii.org");
}

int
Application::run(int &argc, char **argv){
  int returnCode = -1;

  if (Application::instance() == 0){
    // Define the application.
    Application::self = new Application(argc,argv);
    Logger* log = wlog(Application::self);

    // Invoke the initialization code.
    self->d_ptr->initialize();
    log->debug("Completed initialization phase.");

    // Start thyself.
    self->start(); 

    // Begin the event loop.
    log->debug("Beginning event loop.");
    returnCode = self->d_ptr->exec();
    log->info(QString("Event loop ended; ended with exit code %1").arg(returnCode));
  }

  return returnCode;
}

void
Application::start(){
  //Q_D(Application);
  Logger* log = wlog(this);
  log->info("Starting.");

  // Privately start the Factory.
  Factory::instance()->start();


  log->info("Started.");
}

void
Application::stop(){
  //Q_D(Application);

  // Privately clean up the Factory.
  Factory::instance()->stop();
}

Version
Application::version() const {
  Version ver;
  ver.major = WINTERMUTE_VERSION_MAJOR;
  ver.minor = WINTERMUTE_VERSION_MINOR;
  ver.patch = WINTERMUTE_VERSION_PATCH;
  ver.state = (Wintermute::Version::DevelopmentStage) WINTERMUTE_VERSION_STAGE;
  ver.stage = WINTERMUTE_VERSION_STAGE_REVISION;

  return ver;
}

Application::~Application(){
}

#include "application.moc"
