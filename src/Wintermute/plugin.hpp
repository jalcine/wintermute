/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
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
 * @endif
 **/

/**
 * @headerfile Wintermute/plugin.hpp <Wintermute/Plugin>
 * @brief      Definition of Wintermute::Plugin class.
 */

#ifndef WINTERMUTE_PLUGIN_HPP
#define WINTERMUTE_PLUGIN_HPP

#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QtPlugin>
#include <Wintermute/Globals>
#include <Wintermute/Version>

namespace Wintermute
{
class PluginPrivate;

/**
 * @brief An abstract definition of a %Plugin in Wintermute.
 * @sa    Wintermute::Factory
 * @sa    Wintermute::PluginProcess
 *
 * The Plugin object is meant as a way to manage the highest of plug-ins
 * that can be mananged in the platform.
 */
/// @note The most direct means of handling plugins is recommended to be 
//        done through the Factory class.
/// @note Move most of ABI determination in this class.
class Plugin : public QObject
{
	Q_OBJECT
  Q_DISABLE_COPY ( Plugin )
	Q_DECLARE_PRIVATE ( Plugin )
	QScopedPointer<PluginPrivate> d_ptr;

  /** @brief Obtains the name of the plugin. */
	Q_PROPERTY ( QString Name          READ name )
  /** @brief Obtains the name of the version. */
	Q_PROPERTY ( Version Version       READ version )
  /** @brief Obtains the name of the system-required version. */
	Q_PROPERTY ( Version SystemVersion READ systemVersion )

  friend class Factory;

protected:
  /** @brief Default constructor for this plugin. */
	explicit Plugin ( );

  /** @brief Used to obtain the configuration for this plug-in. */
	QSettings* configuration() const;

public:
  typedef QPointer<Plugin> Ptr; ///< Pointer type for Plugin.
	virtual ~Plugin();

  /** @brief Signaled to be invoked when this plugin has started. */
	Q_SIGNAL void started();

  /** @brief Signaled to be invoked when this plugin has stopped. */
	Q_SIGNAL void stopped();

	/**
	 * @brief Obtains the unique name of the plugin.
   * @retval QString The name of the plug-in.
	 */
	QString name() const;

	/**
	 * @brief Obtains the versioning object for the plugin.
   * @retval Version The current version of this plugin.
	 */
	Version version() const;

	/**
	 * @brief Obtains the minimum running version of Wintermute required.
   * @retval Version The minimal version of Wintermute for this plugin.
	 */
	Version systemVersion() const;

	/**
	 * @brief Determines if the plugin has been loaded.
   * @retval boolean Determines if the plugin's binary has been loaded.
	 */
	bool isLoaded() const;

	/**
	 * @brief Defines the logic for the activation of the plugin.
   * @sa started()
	 */
	virtual void start() = 0;

	/**
	 * @brief Defines the logic for the deactivation of the plugin.
   * @sa stopped()
	 */
	virtual void stop() = 0;
};

typedef QList<Plugin::Ptr> PluginList;        ///< Represents a list of plugins.
typedef QMap<QString, Plugin::Ptr> PluginMap; ///< Represents a map of plugins.
}

#endif /* WINTERMUTE_PLUGIN_HPP */
