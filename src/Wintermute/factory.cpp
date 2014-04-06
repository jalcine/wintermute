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
#include "plugin_process.hpp"
#include "logging.hpp"
#include "application.hpp"
#include "private/factory.hpp"
#include "plugin_process.hpp"

using namespace Wintermute;
using Wintermute::Factory;
using Wintermute::FactoryPrivate;

Factory::Ptr Factory::self(nullptr);

Factory::Factory() : QObject ( Application::instance() ),
	d_ptr ( new FactoryPrivate )
{
	connect ( wntrApp, SIGNAL ( started() ), SLOT ( start() ) );
	connect ( wntrApp, SIGNAL ( stopped() ), SLOT ( stop() ) );
}

Factory::Ptr
Factory::instance()
{
  if ( !self ) 
    self = Factory::Ptr(new Factory());

	return Factory::self;
}

Plugin::Ptr
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

bool
Factory::isPluginLoaded ( const QString& name ) const
{
	return activePlugins().contains ( name );
}

// TODO: Ensure plug-in loading process.
bool
Factory::loadPlugin ( const QString& id )
{
	Q_D ( Factory );
	Logger* log = wlog(this);
	if ( isPluginLoaded ( id ) ) {
		log->warn ( QString ( "Already loaded plug-in %1." ).arg ( id ) );
		return true;
	}

	QScopedPointer<QPluginLoader> loader (d->obtainBinary ( id ));
	if ( !loader ) {
		log->debug ( QString ( "Couldn't find binary for plugin '%1'." ).
		             arg ( id ) );
		return false;
	}

	log->debug ( QString ( "Found binary for plugin '%1', loading symbols..." ).
	             arg ( id ) );

  Plugin* plugin = qobject_cast<Plugin*> ( loader->instance() );
	if ( !plugin ) {
		log->error ( QString("Failed to load plugin due to %1." ).
		             arg( loader->errorString() ) );
		return false;
	}
	d->active.insert ( id, plugin );
	log->info ( QString ( "Invoking start of %1..." ).arg ( plugin->name() ) );
	plugin->start();
	log->info( "Started " + plugin->name());
	emit plugin->started();
	log->info ( QString ( "%1 is loaded." ).arg ( id ) );
	return true;
}

PluginProcess::Ptr
Factory::spawnPlugin ( const QString& name )
{
  PluginProcess::Ptr process(new PluginProcess ( name ));
	process->start();
	winfo(wntrFactory.data(), QString("Spawned instance of plugin '%1'.").arg(name));
	return process;
}

bool
Factory::unloadPlugin ( const QString& id )
{
	Q_D ( Factory );
  Plugin::Ptr plugin = d->plugin ( id );
	if ( !plugin->isLoaded() ) {
		return true;
	}
	plugin->stop();
	emit plugin->stopped();
	plugin->d_ptr->loader->unload();
	return true;
}

bool
Factory::autoloadPlugins()
{
	Logger* log = wlog ( this );
	const QStringList defaultPlugins = QStringList() << "wintermute-zeromq";
	QVariant autoload = Wintermute::Application::setting ( "Plugins/Autoload",
	                    defaultPlugins );
	QStringList autoloadList = autoload.value<QStringList>();
	QStringList all = availablePlugins();
	log->info ( QString ( "Loading %1 of %2 known plugins..." ).
	            arg ( autoloadList.length() ).arg ( all.length() ) );
	if ( autoloadList.empty() ) {
		log->warn ( "No plug-ins were specified for auto-loading." );
		return true;
	}
	for ( QString plugin : autoloadList ) {
		if ( !all.contains ( plugin ) ) {
			log->info ( QString ( "The plugin '%1' is not available; bailing out." )
			            .arg ( plugin ) );
			unloadAllPlugins();
			return false;
		}
		bool pluginLoaded = loadPlugin ( plugin );
		if ( !pluginLoaded ) {
			log->info ( QString ( "Autoload of %1 failed;" ).arg ( plugin ) +
			            " thus cancelling the auto-loading process." );
			unloadAllPlugins();
			return false;
		}
	}
	return true;
}

bool
Factory::unloadAllPlugins()
{
	for (QString plugin : activePlugins()) {
		if ( !unloadPlugin ( plugin ) ) {
			return false;
		}
	}
	return true;
}

void
Factory::start()
{
	Logger* log = wlog ( this );
	log->info ( "Starting.." );
	autoloadPlugins();
	emit started();
	log->info ( "Started." );
}

void
Factory::stop()
{
	Logger* log = wlog ( this );
	log->info ( "Stopping.." );
	emit stopped();
	unloadAllPlugins();
	log->info ( "Stopped." );
}

Factory::~Factory()
{
}
