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

#include <algorithm>
#include <wintermute-core/logging.hpp>
#include <wintermute-core/method.hpp>
#include <wintermute-core/util/configuration.hpp>
#include "plugin.hpp"
#include "plugin.hh"

using Wintermute::Plugin;
using Wintermute::Method;
using Wintermute::Util::Configuration;
using DaemonPlugin = Wintermute::Daemon::Plugin;
using DaemonModule = Wintermute::Daemon::Module;
using DaemonPluginPrivate = Wintermute::Daemon::PluginPrivate;
using std::for_each;
using std::to_string;
using std::dynamic_pointer_cast;

DaemonPlugin::Plugin() :
  Wintermute::Plugin(WINTERMUTE_DAEMON_PLUGIN_NAME),
  d_ptr(new DaemonPluginPrivate)
{
}

DaemonPlugin::~Plugin()
{
}

bool DaemonPlugin::startup()
{
  startDesignatedPlugins();
  startHeartbeatInstance();
  startModule();
  return true;
}

bool DaemonPlugin::shutdown()
{
  stopModule();
  stopHeartbeatInstance();
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

void DaemonPlugin::startModule()
{
  W_PRV(DaemonPlugin);
  dynamic_pointer_cast<Daemon::Module>(d->module)->startRelay();
}

void DaemonPlugin::stopModule()
{
  W_PRV(DaemonPlugin);
  dynamic_pointer_cast<Daemon::Module>(d->module)->stopRelay();
}

void DaemonPlugin::startDesignatedPlugins()
{
  W_PRV(const DaemonPlugin);
  Configuration::Ptr daemonCfg = d->config();
  list<string> pluginNames = daemonCfg->get("Start/Plugins", list<string>());

  for_each(pluginNames.begin(), pluginNames.end(), [&](const string& pluginName)
  {
    wdebug("Attempting to load plugin " + pluginName + "...");
    Plugin::Ptr designatedPluginPtr = Plugin::find(pluginName);
    wwarn("Loaded designated plugin " + pluginName + "?" +
      to_string((bool)(designatedPluginPtr)));
    wdebug("Loaded plugin '" + pluginName + "'.");
  });

  Plugin::Ptr heartbeatPluginPtr = Plugin::find("wintermute-heartbeat");
  assert(heartbeatPluginPtr);
}

void DaemonPlugin::stopDesignatedPlugins()
{
  W_PRV(const DaemonPlugin);
  Configuration::Ptr daemonCfg = d->config();
  list<string> pluginNames = daemonCfg->get("Start/Plugins", list<string>());

  for_each(pluginNames.begin(), pluginNames.end(), [&](const string& pluginName)
  {
    const bool pluginUnloaded = Plugin::release(pluginName);
    wdebug("Attempt to unload designated plugin " + pluginName + "?" +
      to_string((int)pluginUnloaded));
  });

  const bool releaseHeartbeat = Plugin::release("wintermute-heartbeat");
  assert(releaseHeartbeat);
}
