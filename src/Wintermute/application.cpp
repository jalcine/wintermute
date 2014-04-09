/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
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
 * @endif
 **/

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QSharedPointer>
#include "application.hpp"
#include "version.hpp"
#include "Wintermute/private/application.hpp"
#include "Wintermute/Procedure/module.hpp"

using Wintermute::Arguments;
using Wintermute::Logging;
using Wintermute::Version;
using Wintermute::Application;
using Wintermute::ApplicationPrivate;
using Wintermute::Procedure::Module;

QPointer<Application> Application::self ( nullptr );

Application::Application ( int& argc, char** argv ) :
  QObject(), d_ptr ( new ApplicationPrivate ( argc, argv, this ) )
{
  Application::self = qobject_cast<Application*> ( this );
  Q_D ( Application );

  setParent(d->app.data());
  d->app->setApplicationName    ( WINTERMUTE_NAME );
  d->app->setApplicationVersion ( version().toString() );
  d->app->setOrganizationName   ( WINTERMUTE_NAME );
  d->app->setOrganizationDomain ( WINTERMUTE_DOMAIN );
  d->settings.reset( new QSettings );
  d->settings->setValue( "Timing/StartupTime" ,
                         QDateTime::currentDateTimeUtc().toString() );
  winfo(this, QString( "Wintermute recorded startup at %1. Hello there!" )
        .arg( d->settings->value( "Timing/StartupTime" ).toString() ) );
  d->installEventFilters();
}

int
Application::run ( int& argc, char** argv )
{
  Q_ASSERT ( Application::instance() == nullptr );
  int returnCode = 0x0;
  if ( Application::instance() == nullptr )
  {
    self = new Application ( argc, argv );
    Logger* log = wlog ( self );
    self->d_ptr->initialize();
    log->info ( QString ( "Wintermute is starting; PID %1. Let's play." ).
                arg ( QCoreApplication::applicationPid() ) );
    self->start();
    log->debug ( "Starting event loop." );
    returnCode = self->d_ptr->exec();
    log->info ( "Event loop ended; ended with " +
                QString ( "exit code %1" ).arg ( returnCode ) );
    self->deleteLater();
    log->deleteLater();
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
  log->info ( QString ( "Stopping Wintermute ..." ) );
  QCoreApplication::quit();
  emit this->stopped();
  log->info ( QString ("Wintermute is stopping with exit code %1." )
              .arg ( exitcode ) );
  exit ( exitcode );
  wntrApp->deleteLater();
}

Module*
Application::module() const
{
  Q_D ( const Application );
  return d->module;
}

Version
Application::version() const
{
  Version ver;
  ver.major = WINTERMUTE_VERSION_MAJOR;
  ver.minor = WINTERMUTE_VERSION_MINOR;
  ver.patch = WINTERMUTE_VERSION_PATCH;
  ver.stage = ( Version::DevelopmentStage )
              WINTERMUTE_VERSION_STAGE;
  ver.hash =  WINTERMUTE_VERSION_STAGE_REF;
  return ver;
}

QVariant
Application::setting ( const QString& path, const QVariant value )
{
  ApplicationPrivate* d = Application::instance()->d_ptr.data();
  return d->settings->contains ( path ) ? d->settings->value ( path ) : value;
}

void
Application::setSetting ( const QString& path, const QVariant value )
{
  // TODO: Should we force the sync?
  ApplicationPrivate* d = Application::instance()->d_ptr.data();
  d->settings->setValue ( path, value );
  d->settings->sync();
}

Application::~Application()
{
  stop();
}
