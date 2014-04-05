/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2013, 2014 Jacky Alciné <me@jalcine.me>
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

#include <QtCore/QSettings>
#include <QtCore/QDateTime>
#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <Wintermute/Application>
#include <Wintermute/Logging>
#include <Wintermute/Factory>
#include <Wintermute/PluginProcess>
#include "module.hpp"
#include "plugin.hpp"
#include "globals.hpp"
#include "module.moc"

using Wintermute::Daemon::Module;
using Wintermute::Daemon::Plugin;

Module::Module ( Daemon::Plugin* plugin ) : Wintermute::Procedure::Module ( plugin )
{
	setDomain ( WINTERMUTE_DOMAIN );
	setPackage ( "daemon" );
}

void
Module::start()
{
	QStringList values = QString(WINTERMUTE_DAEMON_STARTUP_LIST).split(';');
	winfo(this, QString("Plugins to start up: %1.")
	      .arg(WINTERMUTE_DAEMON_STARTUP_LIST));
	startUpPlugins(values);
}

void
Module::stop()
{
}

void
Module::startUpPlugins(const QStringList& plugins)
{
	for (const QString& plugin: plugins) {
		winfo(wntrFactory,
		      QString("Invoking %1 for daemon startup...").arg(plugin));
		wntrFactory->spawnPlugin(plugin);
		winfo(wntrFactory,
		      QString("Started daemon plugin '%1'.").arg(plugin));
	}
}

Module::~Module()
{
}
