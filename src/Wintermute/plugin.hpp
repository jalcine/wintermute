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

#ifndef WINTERMUTE_CORE_PLUGIN_HPP
#define WINTERMUTE_CORE_PLUGIN_HPP

#include <Wintermute/Globals>
#include <Wintermute/Version>
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QtPlugin>

namespace Wintermute
{
class PluginPrivate;

/**
 * @class Wintermute::Plugin
 *
 * The Plugin object is meant as a way to manage the highest of plug-ins
 * that can be mananged in the platform.
 *
 * @note The most direct means of handling plugins is recommended to be done
 * through the Factory class.
 */
class Plugin : public QObject
{
  Q_OBJECT;
  Q_ENUMS ( State );
  Q_PROPERTY ( QString Name          READ name );
  Q_PROPERTY ( Version Version       READ version );
  Q_PROPERTY ( Version SystemVersion READ systemVersion );
  Q_PROPERTY ( State   State         READ state );

  QScopedPointer<PluginPrivate> d_ptr;
  Q_DECLARE_PRIVATE ( Plugin );

protected:
  explicit Plugin ( );
  QSettings* configuration() const;

public:

  virtual ~Plugin();

  Q_SIGNAL void started();
  Q_SIGNAL void stopped();

  /**
   * Flags used to represent the different states that a plug-in can exist
   * in.
   */
  enum State {
    Undefined = 0x0,  // Reserved for a lack of a state (typically null) plugin.
    Loading   = 0x1,  // The plugin is currently undergoing the act of loading its prerequisties into Wintermute.
    Loaded    = 0x2,  // The plugin has been successfully loaded into Wintermute.
    Unloading = 0x3,  // The plugin is currently underdoing the work of removing itself from Wintermute.
    Unloaded  = 0x4,  // The plugin has been successfully removed from Wintermute.
    Crashed   = 0x5   // The plugin has encountered an undefined error.
  };

  /**
   * Defines the kind of plugin that this is.
   */
  enum Type {
    Module    = 0x0, // Defined as a module-based plugin, it'll run in a separate process.
    Addon     = 0x1, // Defined as an add-on plugin, it'll load in its specified parent process.
    Support   = 0x2 // Defined as a support plugin, it'll load in every running Wintermute process.
  };

  /**
   * @fn isLoaded
   * Determines if the plugin has been loaded.
   */
  inline bool isLoaded() const {
    return state() == Loaded;
  }

  /**
   * @fn name
   * Obtains the unique name of the plugin.
   */
  virtual QString name() const = 0;

  /**
   * @fn version
   * Obtains the versioning object for the plugin.
   */
  virtual Version version() const = 0;

  /**
   * @fn systemVersion
   * Obtains the minimum running version of Wintermute required for plugin.
   */
  virtual Version systemVersion() const = 0;

  /**
   * @fn state
   * Obtains the current state of the plugin.
   */
  virtual State state() const = 0;

  /**
   * @fn type
   * Obtains the type of plugin.
   */
  virtual Type type() const = 0;

  /**
   * @fn start
   * Defines the logic for the activation of the plugin.
   */
  virtual void start() = 0;

  /**
   * @fn stop
   * Defines the logic for the deactivation of the plugin.
   */
  virtual void stop() = 0;

  friend class Factory;
  friend class FactoryPrivate;
  friend class TemporaryPlugin;
};

typedef QList<Plugin*> PluginList;
typedef QMap<QString, Plugin*> PluginMap;
}

#endif /* WINTERMUTE_CORE_PLUGIN_HPP */
