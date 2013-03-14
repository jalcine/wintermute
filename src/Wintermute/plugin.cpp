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
#include <QtCore/QPluginLoader>

using Wintermute::Version;
using Wintermute::PluginPrivate;

// {{{ Private data definition
class PluginPrivate : public QObject {
  public:
    QUuid id;
    QSettings* settings;
    QSharedPointer<QPluginLoader> loader;
  
    PluginPrivate(Plugin* parent, const QUuid& uuid) : QObject(parent),
      id(uuid), settings(0), loader(0) {
      }
};
// }}} Private data definition


Wintermute::Plugin::Plugin(const QString& uuid) : QObject(), d_ptr(new PluginPrivate(this, uuid)){
}

// TODO: Provide a means of obtaining a name for plug-ins.
QString
Wintermute::Plugin::name() const {
  return QString::null;
}

// TODO: Provide a means of determining the plugin's version.
Version
Wintermute::Plugin::version() const {
  return Version::Any;
}

// TODO: Provide a means of determining the version of Wintermute required.
Version
Wintermute::Plugin::systemVersion() const {
  return Version::Any;
}

QSettings*
Wintermute::Plugin::configuration() const {
  Q_D(const Plugin);
  return d->settings;
}

bool
Wintermute::Plugin::loadBinary() {
  Q_D(Plugin);
  return false;
}

bool
Wintermute::Plugin::unloadBinary(){
  return false;
}

Wintermute::Plugin::State
Wintermute::Plugin::state() const {
  return Undefined;
}

Wintermute::Plugin::~Plugin(){
}

#include "plugin.moc"
