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

#include "plugin.hpp"
#include "version.hpp"
#include "logging.hpp"
#include "factory.hpp"
#include "application.hpp"
#include "private/plugin.hpp"
#include <QtCore/QUuid>
#include <QtCore/QPluginLoader>

using Wintermute::Version;
using Wintermute::Plugin;
using Wintermute::Logging;
using Wintermute::Logger;

// TODO: Check if loading in its own space, if so, load necessary data.
Plugin::Plugin(const QString& uuid) : QObject(Factory::instance()), d_ptr(new PluginPrivate(uuid)){
}

// TODO: Provide a means of obtaining a name for plug-ins.
QString
Plugin::name() const {
  return QString::null;
}

// TODO: Provide a means of determining the plugin's version.
Version
Plugin::version() const {
  return Version::Any;
}

// TODO: Provide a means of determining the version of Wintermute required.
Version
Plugin::systemVersion() const {
  return Version::Any;
}

QSettings*
Plugin::configuration() const {
  Q_D(const Plugin);
  return d->settings;
}

Plugin::State
Plugin::state() const {
  return Undefined;
}

bool
Plugin::isLoaded() const {
  return false;
}

bool
Plugin::load() {
 return false;
}

bool
Plugin::unload() {
  return false;
}

Plugin::~Plugin(){
}

#include "plugin.moc"
