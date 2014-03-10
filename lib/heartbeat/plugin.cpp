/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
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

#include <Wintermute/Application>
#include "plugin.hpp"
#include "globals.hpp"
#include "modules/pulse.hpp"
#include "modules/monitor.hpp"
#include "plugin.moc"

using Wintermute::Heartbeat::Plugin;
using Wintermute::Version;

Plugin::Plugin() :
  Wintermute::Plugin(),
  module ( 0 )
{
  // TODO There has to be a better way to do this.
  if (wntrApp->modules().length() == 1)
    module = new Heartbeat::MonitorModule(this);
  else
    module = new Heartbeat::PulseModule(this);
}

QString
Plugin::name() const
{
  return "wintermute-heartbeat";
}

void
Plugin::stop()
{
  Q_ASSERT (module != 0);
  module->stop();
}

void
Plugin::start()
{
  Q_ASSERT (module != 0);
  module->start();
}

Version
Plugin::version() const
{
  return Version::fromString ( configuration()->value ( "Version/Plugin" ).toString() );
}

Version
Plugin::systemVersion() const
{
  return Version::fromString ( configuration()->value ( "Version/System" ).toString() );
}

Plugin::State
Plugin::state() const
{
  return Loaded;
}

Plugin::Type
Plugin::type() const
{
  return Support;
}

Q_EXPORT_PLUGIN2 ( wintermute-heartbeat, Wintermute::Heartbeat::Plugin );
