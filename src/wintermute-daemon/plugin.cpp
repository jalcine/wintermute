/*
    Wintermute - A foundation for intelligent computing.
    Copyright (c) 2010 - 2015 by Jacky Alcine

    Wintermute is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    Wintermute is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with Wintermute; if not, write to the
    Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
 */

#include "plugin.hpp"

using Wintermute::Plugin;
using DaemonPlugin = Wintermute::Daemon::Plugin;

DaemonPlugin::Plugin() :
  Wintermute::Plugin(WINTERMUTE_DAEMON_PLUGIN_NAME)
{
}

DaemonPlugin::~Plugin()
{
}

bool DaemonPlugin::startup()
{
  startLighthouseInstance();
  startRelayForTunnel();
  startDesignatedPlugins();
  return true;
}

bool DaemonPlugin::shutdown()
{
  stopLighthouseInstance();
  stopRelayForTunnel();
  stopDesignatedPlugins();
  return true;
}

Plugin::PluginType DaemonPlugin::type() const
{
  return Wintermute::Plugin::PluginTypeService;
}

void DaemonPlugin::startLighthouseInstance()
{
  // TODO: Load the lighthouse plugin.
  // TODO: Send a method call to the module to run in 'warden' mode.
}

void DaemonPlugin::stopLighthouseInstance()
{
  // TODO: Send a method call to the module to kill 'warden' mode.
  // TODO: Unload the lighthouse plugin.
}

void DaemonPlugin::startRelayForTunnel()
{
  // TODO: Send local method call to Tunnel to activate relays.
}

void DaemonPlugin::stopRelayForTunnel()
{
  // TODO: Send local method call to Tunnel to deactivate relays.
}

void DaemonPlugin::startDesignatedPlugins()
{
  // TODO: Fetch list of plugins to be started when
}

void DaemonPlugin::stopDesignatedPlugins()
{
}
