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
 * @headerfile Wintermute/factory.hpp <Wintermute/Factory>
 * @brief      Definition of Wintermute::Factory class.
 */

#ifndef WINTERMUTE_FACTORY_HPP
#define WINTERMUTE_FACTORY_HPP

#include <Wintermute/Globals>
#include <Wintermute/Plugin>
#include <Wintermute/PluginProcess>
#include <QtCore/QStringList>

namespace Wintermute
{
class FactoryPrivate;
class PluginProcess;

/**
 * @brief Handles the loading and manufacturing of plug-ins.
 *
 * Factory is responsible for handling each plug-in that interacts with this
 * process. It handles the [loading][] and [unloading][] of plug-ins as well
 * as determining which plug-ins are designated for [auto-loading][].
 *
 * ## Loading A %Plugin
 *
 * The start of a plugin's load begins with its name. We'll use
 * `wintermute-heartbeat` for this case, but it can be any name that can also
 * be used for a library name. Factory does the following:
 *
 *  - **Locates Metadata** Obtain the plugin's meta information, as much as
 *  possible. At the moment, the only two bits of data we'd need is the
 *  version of the plug-in and the system version. This is how we determine
 *  whether or not loading this plug-in would cause a loading issue.
 *
 *  - **Check ABI**: Determine whether or not this plug-in will work properly in
 *    this version of Wintermute. Simple decimal number comparison.
 *
 *  - **Load binary**: Loads the binary.
 *
 * [loading]: @ref Factory::loadPlugin
 * [unloading]: @ref Factory::unloadPlugin
 * [auto-loading]: @ref Factory::autoloadPlugins
 */
/// @bug Remove the need to use files to load plug-in information.
class Factory : public QObject
{
	friend class Application;
	Q_OBJECT
  Q_DISABLE_COPY(Factory)
	Q_DECLARE_PRIVATE(Factory)
	QScopedPointer<FactoryPrivate> d_ptr;

	explicit Factory();
	Q_SLOT void start();
	Q_SLOT void stop();
	Q_SIGNAL void started();
	Q_SIGNAL void stopped();

public:
  typedef QPointer<Factory> Ptr; ///< Pointer guard for Factory.
	virtual ~Factory();

	/**
   * @brief Obtains the singleton instance of Factory.
   * @retval Factory* A pointer to the singleton instance.
	 */
	static Factory::Ptr instance();

	/**
	 * @brief A list of all of the plugins that Wintermute can load.
   * @retval QStringList A list of the names of all of the known plugins.
   * @deprecated
   * @note This won't be possible when we move to a full binary setup of data
   *       storage about plug-in information.
	 */
	QStringList availablePlugins() const;

	/**
	 * @brief Obtains a list of actively loaded plugins in this process.
   * @retval QStringList A list of the names of the currently loaded plugins.
	 */
	QStringList activePlugins() const;

	/**
	 * Obtains the specified plugin.
	 * @param[in] name The name of the plugin in question.
   * @retval Plugin* A pointer to the plugin.
   * @note Consider using a pointer wrapper here.
	 */
  Plugin::Ptr plugin ( const QString& name ) const;

	/**
	 * @brief Loads the specified plugin into this Wintermute process.
	 * @param[in] name The name of the plug-in.
   * @retval boolean Whether or not the plugin was loaded.
   * @note Consider taking a output parameter to also provide error codes.
	 */
	bool loadPlugin ( const QString& name );

	/**
	 * @brief Unloads the specified plugin from this Wintermute process.
	 * @param[in] name The name of the plug-in.
   * @retval boolean Whether or not the plugin was unloaded.
   * @note Consider taking a output parameter to also provide error codes.
	 */
	bool unloadPlugin ( const QString& name );

	/**
	 * @brief Loads the specified plugin in a separate process.
	 * @param[in] name The name of the plug-in.
   * @retval PluginProcess::Ptr A pointer to the newly created plugin process.
   * @note Consider taking a output parameter to also provide error codes.
	 */
  PluginProcess::Ptr spawnPlugin ( const QString& name );

	/**
   * @brief Determines if the provided plug-in is loaded.
	 * @param[in] plugin The name of the plugin in question.
   * @retval boolean Whether or not the plugin is loaded.
	 */
	bool isPluginLoaded ( const QString& plugin ) const;

	/**
   * @brief Loads the plugins corresponding to Wintermute's mode.
   * @retval boolean Whether or not this operation completely successfully.
   *
	 * Automatically loads the appropriate plugins for this process. This
	 * operation changes on the following cases:
	 *
	 *  * `--mode=daemon`: (loads the daemon plugin)
	 *  * `--mode=plugin`: (loads the plugin as specified by `--plugin`)
   *
   * @note Consider taking a output parameter to also provide error codes.
	 */
	bool autoloadPlugins();

	/**
	 * @brief  Unloads all of the active plugins in the process.
   * @retval boolean Whether or not this operation completely successfully.
   * @note Consider taking a output parameter to also provide error codes.
	 */
	bool unloadAllPlugins();
private:
	static Factory::Ptr self;
};
}

/**
 * @brief Auxiliary method to Factory's singleton.
 */
#define wntrFactory Wintermute::Factory::instance()

#endif /* WINTERMUTE_FACTORY_HPP */
