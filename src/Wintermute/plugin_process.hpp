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
 * @headerfile Wintermute/plugin_process.hpp <Wintermute/PluginProcess>
 * @brief      Definition of Wintermute::PluginProcess class.
 */


#ifndef WINTERMUTE_PLUGIN_PROCESS_HPP
#define WINTERMUTE_PLUGIN_PROCESS_HPP

#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <Wintermute/Plugin>

namespace Wintermute
{
class PluginProcessPrivate;
class Factory;

/**
 * @brief Provides a means of controlling sub-plugins.
 * @sa    Factory::spawnPlugin(const QString&)
 *
 * This class allows one to control plug-ins that are started in a child
 * process.
 */
class PluginProcess : public QObject
{
  friend class Factory;
	Q_OBJECT
	Q_DECLARE_PRIVATE ( PluginProcess )
  Q_DISABLE_COPY ( PluginProcess )
	explicit PluginProcess(const QString& pluginName);
	QScopedPointer<PluginProcessPrivate> d_ptr;

public:
  typedef QPointer<PluginProcess> Ptr; ///< Pointer for PluginProcess.
	virtual ~PluginProcess();            ///< Destructor.
  /**
   * @brief Obtains the name of the plugin running in the child process.
   * @retval QString The name of the child plugin.
   * @sa PluginProcess::pid()
   */
	QString pluginName() const;

  /**
   * @brief Obtains the PID of the child process.
   * @retval Q_PID The platform-dependent implementation of the child plugin.
   * @sa PluginProcess::name()
   */
  Q_PID pid() const;

	Q_SLOT void start();      ///< Starts the child process.
	Q_SLOT void stop();       ///< Stops the child process.
  Q_SIGNAL void started();  ///< Signal invoked when started.
  Q_SIGNAL void stopped();  ///< Signal invoked when stopped.

};
}

#endif
