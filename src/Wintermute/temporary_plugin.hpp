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

#ifndef WINTERMUTE_CORE_INTERNAL_TEMPORARYPLUGIN_HPP
#define WINTERMUTE_CORE_INTERNAL_TEMPORARYPLUGIN_HPP

#include <QtCore/QUuid>
#include <QtCore/QPluginLoader>
#include <Wintermute/Plugin>
#include <Wintermute/Version>

namespace Wintermute
{
class PluginInterfaceable;
class TemporaryPlugin : public Plugin
{
  Q_OBJECT;
  QUuid m_id;

public:
  explicit TemporaryPlugin ( const QUuid& id, QPluginLoader* theLoader );
  virtual ~TemporaryPlugin();
  PluginInterfaceable* tryLoad ( QPluginLoader* loader );
  virtual inline QUuid id() const { return m_id; }
  virtual inline QString name() const { return name(); }
  virtual Version version() const { return Version::Any; }
  virtual Version systemVersion() const { return Version::Any; }
  virtual Plugin::State state() const { return Undefined; }
  virtual Plugin::Type type() const { return Module; }
};
}

#endif /* WINTERMUTE_CORE_INTERNAL_TEMPORARYPLUGIN_HPP */
