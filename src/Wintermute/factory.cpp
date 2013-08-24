/**
 *
 * Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
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

#include "factory.hpp"
#include "plugin.hpp"
#include "logging.hpp"
#include "application.hpp"
#include "temporary_plugin.hpp"
#include "private/factory.hpp"
#include <QDebug>

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
	if ( !self )
		{ self = new Factory; }
	return self;
}

PluginList
Factory::availablePlugins() const
{
	Q_D ( const Factory );
	return d->availablePlugins();
}

PluginList
Factory::activePlugins() const
{
	Q_D ( const Factory );
	return d->activePlugins();
}

// TODO: Ensure plug-in loading process.
bool
Factory::loadPlugin ( const QUuid& id )
{
	Q_D ( Factory );
	PluginInterfaceable* obtainedPluginInterface = 0;
	Logger* log = wlog ( this );
	QPluginLoader* loader = d->obtainBinary ( id );
	TemporaryPlugin* plugin = new TemporaryPlugin ( id, loader );
	if ( !loader ) {
		log->debug ( QString ( "Couldn't find binary for plugin '%1'." ).arg ( id.toString() ) );
		return false;
	} else {
		log->debug ( QString ( "Found binary for plugin '%1'." ).arg ( id.toString() ) );
	}
	log->info ( QString ( "Attempted to load plug-in instance for %1..." ).arg ( id.toString() ) );
	obtainedPluginInterface = plugin->tryLoad ( loader );
	if ( obtainedPluginInterface == 0 ) {
		log->error ( QString ( "Plug-in instance for %1 failed to load due to '%2'." ).arg ( id.toString() ).arg ( loader->errorString() ) );
	}
	return obtainedPluginInterface;
}

// TODO: Verify the unloading process.
bool
Factory::unloadPlugin ( const QUuid& id )
{
	Q_D ( Factory );
	Plugin* plugin = d->plugin ( id );
	if ( !plugin->isLoaded() ) {
		return true;
	}
	return plugin->unload();
}

// TODO: Auto load plugins on start.
bool
Factory::autoloadPlugins()
{
	Logger* log = wlog ( this );
	QVariantList autoloadList = Wintermute::Application::setting ( "Plugins/Autoload", QVariantList() ).toList();
	PluginList all = this->availablePlugins();
	log->info ( QString ( "Loading %1 of %2 plugins..." ).arg ( autoloadList.length() ).arg ( all.length() ) );
	Q_FOREACH ( Plugin * plugin, all ) {
		bool pluginLoaded = this->loadPlugin ( plugin->id().toString() );
		if ( !pluginLoaded ) {
			log->info ( QString ( "Autoload of %1 failed; thus cancelling the auto-loading process." ).arg ( plugin->id().toString() ) );
			//return false;
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
Factory::pluginStateChange ( const QUuid& id, const Plugin::State& state )
{
	Logger* log = wlog ( this );
	log->info ( "Passing signal." );
	emit this->pluginStateChanged ( id, state );
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

#include "Wintermute/factory.moc"
