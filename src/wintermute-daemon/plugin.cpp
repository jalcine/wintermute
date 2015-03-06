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

#include <wintermutecore/logging.hpp>
#include <wintermutecore/method.hpp>
#include "plugin.hpp"
#include "plugin.hh"

using Wintermute::Plugin;
using Wintermute::Method;
using DaemonPlugin = Wintermute::Daemon::Plugin;
using DaemonPluginPrivate = Wintermute::Daemon::PluginPrivate;

DaemonPlugin::Plugin() :
  Wintermute::Plugin(WINTERMUTE_DAEMON_PLUGIN_NAME)
{
}

DaemonPlugin::~Plugin()
{
}

bool DaemonPlugin::startup()
{
  startHeartbeatInstance();
  startRelayForTunnel();
  startDesignatedPlugins();
  return true;
}

bool DaemonPlugin::shutdown()
{
  stopHeartbeatInstance();
  stopRelayForTunnel();
  stopDesignatedPlugins();
  return true;
}

Plugin::PluginType DaemonPlugin::type() const
{
  return Wintermute::Plugin::PluginTypeService;
}

void DaemonPlugin::startHeartbeatInstance()
{
  wdebug("Starting up heartbeat instance...");
  W_PRV(DaemonPlugin);
  d->loadHeartbeat();
  wdebug("Started heartbeat instance.");
}

void DaemonPlugin::stopHeartbeatInstance()
{
  wdebug("Stopping heartbeat instance...");
  W_PRV(DaemonPlugin);
  d->unloadHeartbeat();
  wdebug("Stopped heartbeat instance.");
}

void DaemonPlugin::startRelayForTunnel()
{
  // TODO: Send local method call to Tunnel to activate relays.
  Method::Ptr methodCall = std::make_shared<Method>(
      "startRelay",
      Module::Designation("in.wintermute", "zeromq"),
      Module::Designation("in.wintermute", "daemon")
  );
}

void DaemonPlugin::stopRelayForTunnel()
{
  // TODO: Send local method call to Tunnel to deactivate relays.
}

void DaemonPlugin::startDesignatedPlugins()
{
  // TODO: Fetch list of plugins to be started when Wintermute is started.
  // TODO: For each plugin in #1, load plugin.
  // TODO: For each plugin in #1, confirm successful plugin load.
}

void DaemonPlugin::stopDesignatedPlugins()
{
  // TODO: Fetch list of plugins to be started when Wintermute is started.
  // TODO: For each plugin in #1, unload plugin.
  // TODO: For each plugin in #1, confirm successful plugin unload.
}
