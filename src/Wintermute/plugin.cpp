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

#include <QtCore/QSettings>
#include <QtCore/QPluginLoader>
#include <QtCore/QMetaClassInfo>
#include "plugin.hpp"
#include "version.hpp"
#include "logging.hpp"
#include "factory.hpp"
#include "application.hpp"
#include "private/plugin.hpp"

using Wintermute::Version;
using Wintermute::Plugin;
using Wintermute::Logging;
using Wintermute::Logger;

Plugin::Plugin ( ) :
	QObject ( Factory::instance() ),
	d_ptr ( new PluginPrivate ( this ) )
{
}

QSettings*
Plugin::configuration() const
{
	Q_D ( const Plugin );
	if ( d->settings == 0 ) {
		QMetaClassInfo nameClass = metaObject()->classInfo (
		                             metaObject()->indexOfClassInfo ( "Name" ) );
		const QString name = nameClass.value();
		d->settings = new QSettings( "Wintermute", name, parent() );
		winfo( this, QString( "%1's settings are now found at %2." ).arg( name,
		       d->settings->fileName() ) );
	}
	return d->settings;
}

Version
Plugin::version() const
{
	QVariant value = configuration()->value ( "Version/Plugin" );
	return value.isNull() ? Version() : Version::fromString ( value.toString() );
}

Version
Plugin::systemVersion() const
{
	QVariant value = configuration()->value ( "Version/System" );
	return value.isNull() ? Version() : Version::fromString ( value.toString() );
}

QString
Plugin::name() const
{
	QMetaClassInfo nameClass = metaObject()->classInfo (
	                             metaObject()->indexOfClassInfo ( "Name" ) );
	QVariant value = configuration()->value("Plugin/Name");
	return value.isNull() ? nameClass.value() : value.toString();
}

bool
Plugin::isLoaded() const
{
	Q_D ( const Plugin );
	return d->loader != 0 && d->loader->isLoaded();
}

Plugin::~Plugin()
{
}
