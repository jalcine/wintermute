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
#include "Wintermute/globals.hpp"
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

using Wintermute::Arguments;
using Wintermute::Logging;
using Wintermute::Application;
using Wintermute::ApplicationPrivate;

class ApplicationPrivate {
  public:
    QSharedPointer<QCoreApplication> app;

    ApplicationPrivate(int &argc, char **argv) {
      app = QSharedPointer<QCoreApplication>(new QCoreApplication(argc,argv));
    }

    void
    initialize(){
      // Allocate necessary variables for logging and arguments.
      Logging* logging     = Logging::self   = new Logging;
      Arguments* arguments = Arguments::self = new Arguments;
    }

    int
    exec(){
      return app->exec();
    }
};

Application* Application::self = 0;

Application::Application(int &argc, char **argv) : QObject(), d_ptr(new ApplicationPrivate(argc,argv)){
  Q_D(Application);

  // I am self!
  Application::self = qobject_cast<Application*>(this);

  // Define the application in Qt.
  d->app->setApplicationName("Wintermute");
  d->app->setApplicationVersion(WINTERMUTE_VERSION_EXACT);
  d->app->setOrganizationName("Synthetic Intellect Institute");
  d->app->setOrganizationDomain("thesii.org");
}

int
Application::run(int &argc, char **argv){
  if (Application::instance() == 0){
    // Define the application.
    Application::self = new Application(argc,argv);

    // Invoke the initialization code.
    self->d_ptr->initialize();
    Logging::obtainLogger(Application::self)->debug("Completed initialization phase.");

    // Start thyself.
    self->start(); 
    Logging::obtainLogger(Application::self)->debug("Started.");

    // Begin the event loop.
    Logging::obtainLogger(Application::self)->debug("Beginning event loop.");
    int returnCode = self->d_ptr->exec();
    Logging::obtainLogger(Application::self)->info("Event loop ended. Ended with exit code " + QString::number(returnCode));

    return returnCode;
  }

  return -1;
}

void
Application::start(){
  Logger* log = Logging::obtainLogger(this);
}


void
Application::stop(){
}

Application::~Application(){
}

#include "application.moc"
