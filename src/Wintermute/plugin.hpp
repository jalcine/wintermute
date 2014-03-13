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

#ifndef WINTERMUTE_PLUGIN_HPP
#define WINTERMUTE_PLUGIN_HPP

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
   * @fn isLoaded
   * Determines if the plugin has been loaded.
   */
  bool isLoaded() const;

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

#endif /* WINTERMUTE_PLUGIN_HPP */
