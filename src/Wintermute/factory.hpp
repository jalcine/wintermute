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

#ifndef WINTERMUTE_CORE_FACTORY_HPP
#define WINTERMUTE_CORE_FACTORY_HPP

#include <Wintermute/Globals>
#include <Wintermute/Plugin>
#include <QtCore/QStringList>

namespace Wintermute
{
class FactoryPrivate;

/**
 * @class Factory
 * @brief Handles the loading and manufacturing of plug-ins.
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
   * @function Factory
   * @constructor
   *
   */
  explicit Factory();

private:
  Q_SLOT void start();
  Q_SLOT void stop();
  Q_SLOT void pluginStateChange ( const QString& name, const Plugin::State& state );
  Q_SIGNAL void started();
  Q_SIGNAL void stopped();

public:
  /**
   * @function ~Factory
   * @destructor
   */
  virtual ~Factory();

  /**
   * @function instance
   * @static
   */
  static Factory* instance();

  /**
   * @function availablePlugins
   *
   * A list of all of the plugins that Wintermute can load.
   */
  QStringList availablePlugins() const;

  /**
   * @function activePlugins
   *
   * Obtains a list of actively loaded plugins in this process.
   */
  QStringList activePlugins() const;

  /**
   * @function plugin
   * @param name
   *
   * Obtains the specified plugin.
   */
  Plugin* plugin ( const QString& name ) const;

  /**
   * @function loadPlugin
   * @param name The name of the plug-in.
   *
   * Loads the specified plugin into this Wintermute process.
   */
  bool loadPlugin ( const QString& name );

  /**
   * @function unloadPlugin
   * @param name The name of the plug-in.
   *
   * Unloads the specified plugin from this Wintermute process.
   */
  bool unloadPlugin ( const QString& name );

  bool loadedPlugin( const QString& name );
  /**
   * @function autoloadPlugins
   *
   * Automatically loads the appropriate plugins for this process. This
   * operation changes on the following cases:
   *
   *  + --mode=daemon (loads the daemon plugin)
   *  + --mode=plugin (loads the plugin as specified by --plugin)
   */
  bool autoloadPlugins();

  /**
   * @function unloadAllPlugins
   *
   * Unloads all of the plugins that are currently running in this process.
   */
  bool unloadAllPlugins();

  /**
   * @function pluginStateChanged
   *
   * Raised when a plugin state has changed. This is risen for every plugin so
   * it's recommended that you listen specifically for the plug-in in
   * question by the 'name' parameter.
   */
  Q_SIGNAL void pluginStateChanged ( const QString& name, const Plugin::State& state );
};
}

#define wntrFactory Wintermute::Factory::instance()

#endif /* WINTERMUTE_CORE_FACTORY_HPP */
