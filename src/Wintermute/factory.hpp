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

#ifndef WINTERMUTE_CORE_FACTORY_HPP
#define WINTERMUTE_CORE_FACTORY_HPP

#include <Wintermute/Globals>
#include <Wintermute/Plugin>
#include <Wintermute/PluginInterfaceable>
#include <QtCore/QStringList>
#include <QtCore/QUuid>

namespace Wintermute
{
class FactoryPrivate;

/**
 * @class Factory
 * @brief Handles the loading and manufactoring of plug-ins.
 *
 * In the core module state, plug-ins are loaded in the following fashion:
 *
 *  - Grab metadata: Obtain the plug-in's meta information, as much as
 *  possible. At the moment, the only two bits of data we'd need is the
 *  version of the plug-in and the ABI version. This is how we determine
 *  whether or not loading this plug-in would cause a loading issue.
 *
 *  - Check ABI: Determine whether or not this plug-in will work properly in
 *  this version of Wintermute. Simple decimal number comparison.
 *
 *  - Load binary: Loads the binary.
 */
class Factory : public QObject
{
  friend class Application;
  friend class ApplicationPrivate;

  Q_OBJECT;
  Q_DECLARE_PRIVATE ( Factory );
  FactoryPrivate* d_ptr;
  static Factory* self;

  /**
   * @fn Factory
   * @constructor
   *
   */
  explicit Factory();

private:
  void start();
  void stop();
  Q_SLOT void pluginStateChange ( const QUuid& id, const Plugin::State& state );

public:
  /**
   * @fn ~Factory
   * @destructor
   */
  virtual ~Factory();

  /**
   * @fn instance
   * @static
   */
  static Factory* instance();
  PluginList availablePlugins() const;
  PluginList activePlugins() const;
  bool loadPlugin ( const QUuid& id );
  bool unloadPlugin ( const QUuid& id );
  bool autoloadPlugins();
  bool unloadAllPlugins();

  Q_SIGNAL void pluginStateChanged ( const QUuid& id, const Plugin::State& state );
};
}

#define wntrFactory Wintermute::Factory::instance()

#endif /* WINTERMUTE_CORE_FACTORY_HPP */
