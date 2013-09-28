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

#include <QtCore/QDebug>
#include "factory.hpp"
#include "plugin.hpp"
#include "logging.hpp"
#include "application.hpp"
#include "private/factory.hpp"
#include "Wintermute/factory.moc"

using namespace Wintermute;
using Wintermute::Factory;
using Wintermute::FactoryPrivate;

Factory* Factory::self = 0;

Factory::Factory() : QObject ( Application::instance() ), d_ptr ( new FactoryPrivate )
{
}

Factory*
Factory::instance()
{
  if ( !self ) {
    self = new Factory;
  }
  return self;
}

Plugin*
Factory::plugin ( const QString& name ) const
{
  Q_D ( const Factory );
  return d->plugin ( name );
}

QStringList
Factory::availablePlugins() const
{
  Q_D ( const Factory );
  return d->availablePlugins();
}

QStringList
Factory::activePlugins() const
{
  Q_D ( const Factory );
  return d->activePlugins();
}

// TODO: Ensure plug-in loading process.
bool
Factory::loadPlugin ( const QString& id )
{
  Q_D ( Factory );
  Logger* log = wlog ( this );
  QPluginLoader* loader = d->obtainBinary ( id );
  Plugin* plugin = 0;
  if ( !loader ) {
    log->debug ( QString ( "Couldn't find binary for plugin '%1'." ).arg ( id ) );
    return false;
  }
  log->debug ( QString ( "Found binary for plugin '%1'." ).arg ( id ) );
  plugin = qobject_cast<Plugin*> ( loader->instance() );
  if ( !plugin ) {
    log->error ( "Failed to load plugin." );
    log->error ( loader->errorString() );
    return false;
  }
  d->active.insert ( id, plugin );
  connect ( wntrApp, SIGNAL ( started() ), plugin, SLOT ( start() ) );
  log->info ( QString ( "%1 is loaded." ).arg ( id ) );
  return true;
}

// TODO: Verify the unloading process.
bool
Factory::unloadPlugin ( const QString& id )
{
  Q_D ( Factory );
  Plugin* plugin = d->plugin ( id );
  if ( !plugin->isLoaded() ) {
    return true;
  }
  plugin->stop();
  plugin->d_ptr->loader->unload();
  return true;
}

// TODO: Auto load plugins on start.
bool
Factory::autoloadPlugins()
{
  Logger* log = wlog ( this );
  QVariantList autoloadList = Wintermute::Application::setting ( "Plugins/Autoload", QVariantList() ).toList();
  QStringList all = this->availablePlugins();
  log->info ( QString ( "Loading %1 of %2 plugins..." ).arg ( autoloadList.length() ).arg ( all.length() ) );
  Q_FOREACH ( QString plugin, all ) {
    bool pluginLoaded = this->loadPlugin ( plugin );
    if ( !pluginLoaded ) {
      log->info ( QString ( "Autoload of %1 failed; thus cancelling the auto-loading process." ).arg ( plugin ) );
      this->unloadAllPlugins();
      return false;
    }
  }
  return true;
}

bool
Factory::unloadAllPlugins()
{
  return false;
}

void
Factory::pluginStateChange ( const QString& name, const Plugin::State& state )
{
  Logger* log = wlog ( this );
  log->info ( "Passing signal." );
  emit this->pluginStateChanged ( name, state );
}

void
Factory::start()
{
  Logger* log = wlog ( this );
  log->info ( "Starting.." );
  //this->autoloadPlugins();
  log->info ( "Started." );
}

void
Factory::stop()
{
  Logger* log = wlog ( this );
  log->info ( "Stopping.." );
  this->unloadAllPlugins();
  log->info ( "Stopped." );
}

Factory::~Factory()
{
}

