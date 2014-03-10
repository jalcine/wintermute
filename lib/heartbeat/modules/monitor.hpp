/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2014 Jacky Alciné <me@jalcine.me>
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

#ifndef WINTERMUTE_HEARTBEAT_MONITOR_MODULE_HPP 
#define WINTERMUTE_HEARTBEAT_MONITOR_MODULE_HPP 

#include <Wintermute/Procedure/Module>

namespace Wintermute
{
namespace Heartbeat
{
class Plugin;

/**
 * @class MonitorModule
 * @brief Represents the monitoring instance of the heartbeat plugin.
 *
 * The monitor module keeps track of all of the processes on this local
 * machine that send a signal to this instance. This is done by recording the
 * initial 'pulse' that comes from a PulseModule that's embedded into every
 * process of Wintermute.
 */
class MonitorModulePrivate;
class MonitorModule : public Wintermute::Procedure::Module
{
  Q_OBJECT;
  Q_DISABLE_COPY ( MonitorModule );
  Q_DECLARE_PRIVATE ( MonitorModule );
  QScopedPointer<MonitorModulePrivate> d_ptr;

public:
  explicit MonitorModule ( Heartbeat::Plugin* plugin );
  virtual ~MonitorModule();
  Q_SLOT virtual void start();
  Q_SLOT virtual void stop();

  /**
   * @fn pids
   * @brief Gets the known PIDS of Wintermute.
   *
   * Gets a list of the reporting PIDs that are currently running processes of
   * Wintermute.
   */
  QList<qint64> pids() const;
};
}
}

#endif /* */
