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

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QSharedPointer>
#include "application.hpp"
#include "version.hpp"
#include "Wintermute/private/application.hpp"
#include "Wintermute/Procedure/module.hpp"
#include "Wintermute/application.moc"

using Wintermute::Arguments;
using Wintermute::Logging;
using Wintermute::Version;
using Wintermute::Application;
using Wintermute::ApplicationPrivate;
using Wintermute::Procedure::Module;

Application* Application::self = 0;

Application::Application ( int& argc, char** argv ) :
  QObject(), d_ptr ( new ApplicationPrivate ( argc, argv, this ) )
{
  Q_D ( Application );
  Application::self = qobject_cast<Application*> ( this );
  d->app->setApplicationName ( WINTERMUTE_NAME );
  d->app->setApplicationVersion ( this->version().toString() );
  d->app->setOrganizationName ( WINTERMUTE_NAME );
  d->app->setOrganizationDomain ( WINTERMUTE_DOMAIN );
  d->settings = new QSettings;
}

int
Application::run ( int& argc, char** argv )
{
  int returnCode = -1;
  if ( Application::instance() == 0 )
  {
    Application::self = new Application ( argc, argv );
    Logger* log = wlog ( Application::self );
    self->d_ptr->initialize();
    log->info(QString("Wintermute is starting; PID %1. Let's play.").
        arg(QCoreApplication::applicationPid()));
    self->start();

    log->debug ( "Starting event loop." );
    returnCode = self->d_ptr->exec();
    log->info ( "Event loop ended; ended with" +
        QString( "exit code %1" ).arg ( returnCode ) );
  }
  return returnCode;
}

void
Application::start()
{
  Q_D ( Application );
  Logger* log = wlog ( this );
  log->info ( "Starting Wintermute's process module..." );
  d->loadProcessModule();
  log->info ( "Starting Wintermute's current mode..." );
  d->loadCurrentMode();
  log->info ( "Invoking post-start logic..." );
  emit this->started();
  log->info ( "Started Wintermute." );
}

void
Application::stop ( int exitcode )
{
  Logger* log = wlog ( this );
  log->info ( "Stopping Wintermute..." );
  QCoreApplication::quit();
  emit this->stopped();
  log->info ( "Wintermute is stopping " + QString("with exit code %1.")
      .arg(exitcode) );

  if (QCoreApplication::startingUp() || QCoreApplication::closingDown()){
    exit ( exitcode );
  }
}

QString
Application::processName() const
{
  Q_D ( const Application );
  if ( !d->module )
  {
    return QString::null;
  }
  return d->module->qualifiedName();
}

QList<Module*>
Application::modules() const
{
  Q_D ( const Application );
  return d->modules;
}

Module*
Application::findModule ( const QString& name ) const
{
  // NOTE: This could be so much more functional.
  Q_D ( const Application );
  Q_FOREACH(Module * mod, d->modules)
  {
    if ( mod->domain().contains ( name ) )
    {
      return mod;
    }
  }
  return nullptr;
}

Version
Application::version() const
{
  Version ver;
  ver.major = WINTERMUTE_VERSION_MAJOR;
  ver.minor = WINTERMUTE_VERSION_MINOR;
  ver.patch = WINTERMUTE_VERSION_PATCH;
  ver.state = ( Wintermute::Version::DevelopmentStage ) WINTERMUTE_VERSION_STAGE;
  ver.stage = QString ( "%1:%2" ).arg ( WINTERMUTE_VERSION_STAGE_REF, WINTERMUTE_VERSION_STAGE_BRANCH );
  return ver;
}

QVariant
Application::setting ( const QString& path, const QVariant defaultValue )
{
  ApplicationPrivate* d = Application::instance()->d_ptr.data();
  d->settings->contains ( path ) ? d->settings->value ( path ) : defaultValue
  return defaultValue;
}

void
Application::setSetting ( const QString& path, const QVariant value )
{
  ApplicationPrivate* d = Application::instance()->d_ptr.data();
  d->settings->setValue ( path, value );
}

Application::~Application()
{
  this->stop();
  this->deleteLater();
  winfo(this, "Application singleton deleted.");
}
