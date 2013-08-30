/**
 * vim: ft=cpp
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

#include "application.hpp"
#include "version.hpp"
#include "Wintermute/private/application.hpp"
#include "Wintermute/Procedure/module.hpp"
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QSharedPointer>

using Wintermute::Arguments;
using Wintermute::Logging;
using Wintermute::Version;
using Wintermute::Application;
using Wintermute::ApplicationPrivate;
using Wintermute::Procedure::Module;

Application* Application::self = 0;

Application::Application ( int& argc, char** argv ) : QObject(), d_ptr ( new ApplicationPrivate ( argc, argv ) )
{
  Q_D ( Application );
  // I am self!
  Application::self = qobject_cast<Application*> ( this );
  // Define the application in Qt.
  d->app->setApplicationName ( "Wintermute" );
  d->app->setApplicationVersion ( this->version().toString() );
  d->app->setOrganizationName ( "Wintermute" );
  d->app->setOrganizationDomain ( "jalcine.me" );
  // Grab our settings.
  d->settings = new QSettings;
}

int
Application::run ( int& argc, char** argv )
{
  int returnCode = -1;
  if ( Application::instance() == 0 ) {
    // Define the application.
    Application::self = new Application ( argc, argv );
    Logger* log = wlog ( Application::self );
    // Invoke the initialization code.
    self->d_ptr->initialize();
    // Start thyself.
    self->start();
    // Begin the event loop.
    log->debug ( "Starting event loop." );
    returnCode = self->d_ptr->exec();
    log->info ( QString ( "Event loop ended; ended with exit code %1" ).arg ( returnCode ) );
  }
  return returnCode;
}

void
Application::start()
{
  Q_D ( Application );
  Logger* log = wlog ( this );
  log->info ( "Starting Wintermute..." );
  d->loadProcessModule();
  d->loadCurrentMode();
  emit this->started();
}

void
Application::stop ( int exitcode )
{
  Logger* log = wlog ( this );
  log->info ( "Qutting application..." );
  QCoreApplication::quit();
  emit this->stopped();
  exit ( exitcode );
}

QString
Application::processName() const
{
  Q_D ( const Application );
  if ( !d->module )
    { return QString::null; }
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
  Q_D ( const Application );
  for ( int i = 0; i < d->modules.size(); ++i ) {
    Module* mod = d->modules.at ( i );
    if ( mod->domain().contains ( name ) )
      { return mod; }
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
  if ( d->settings->contains ( path ) )
    { return d->settings->value ( path ); }
  else
    { return defaultValue; }
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
}

#include "Wintermute/application.moc"
