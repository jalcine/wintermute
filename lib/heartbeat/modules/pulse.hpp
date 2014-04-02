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

#ifndef HEARTBEAT_PULSE_MODULE_HPP
#define HEARTBEAT_PULSE_MODULE_HPP

#include <Wintermute/Procedure/Module>

namespace Wintermute
{
namespace Heartbeat
{
class Plugin;

/**
 * @class PulseModule
 * @brief Represents the monitoring instance of the heartbeat plugin.
 *
 * The monitor module keeps track of all of the processes on this local
 * machine that send a signal to this instance. This is done by recording the
 * initial 'pulse' that comes from a PulseModule that's embedded into every
 * process of Wintermute.
 */
class PulseModulePrivate;
class PulseModule : public Wintermute::Procedure::Module
{
  Q_OBJECT;
  Q_DISABLE_COPY ( PulseModule );
  QScopedPointer<PulseModulePrivate> d_ptr;
  Q_DECLARE_PRIVATE ( PulseModule );
  Q_SLOT void tick();

public:
  enum PulseType {
    PulseStartingUp   = 0x001,
    PulseIdle         = 0x002,
    PulseAlive        = 0x003,
    PulseClosingDown  = 0xfff
  };

  Q_ENUMS(PulseType);

  explicit PulseModule ( Heartbeat::Plugin* plugin );
  virtual ~PulseModule();

public slots:
  Q_INVOKABLE virtual void start();
  Q_INVOKABLE virtual void stop();
  Q_INVOKABLE void pulse(PulseType type);
};
}
}

#endif /* */
