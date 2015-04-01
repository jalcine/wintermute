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
#include <wintermute-core/tunnel.hpp>
#include "plugin.hh"
#include "plugin.hpp"
#include "module.hpp"

using Wintermute::Plugin;
using Wintermute::Tunnel;
using Wintermute::Events::Event;
using Wintermute::Events::Listener;
using HeartbeatPlugin = Wintermute::Heartbeat::Plugin;
using HeartbeatModule = Wintermute::Heartbeat::Module;
using HeartbeatPluginPrivate = Wintermute::Heartbeat::PluginPrivate;

HeartbeatPlugin::Plugin() :
  Wintermute::Plugin(WINTERMUTE_HEARTBEAT_PLUGIN_NAME),
  d_ptr(make_shared<HeartbeatPluginPrivate>())
{
  wdebug("Heartbeat plugin created.");
}

HeartbeatPlugin::~Plugin()
{
  wdebug("Heartbeat plugin down for the count.");
}

bool HeartbeatPlugin::startup()
{
  W_PRV(HeartbeatPlugin);

  auto enableModuleFunc = [this](const Event::Ptr& eventPtr)
  {
    assert(eventPtr);
    auto module = make_shared<HeartbeatModule>();
    assert(module);
    wdebug("Starting up the heartbeat module...");
    module->enable();

    this->d_ptr->module = module;
    wdebug("Heartbeat root module enabled.");
    assert(this->d_ptr->module);
  };

  auto disableModuleFunc = [this](const Event::Ptr& eventPtr)
  {
    assert(eventPtr);
    wdebug("Disabling the module..");
    auto module = this->d_ptr->module;

    if (module)
    {
      module->disable();
      module.reset();
      wdebug("Heartbeat root module disabled.");
    }
    else
    {
      wdebug("No heartbeat root module to disable.");
    }

    this->d_ptr->module = nullptr;
    assert(!this->d_ptr->module);
  };

  d->startUpModuleListener = Tunnel::instance()->listenForEvent (
    W_EVENT_TUNNEL_START,
    enableModuleFunc,
    Listener::FrequencyOnce
  );

  d->shutDownModuleListener = Tunnel::instance()->listenForEvent (
    W_EVENT_TUNNEL_STOP,
    disableModuleFunc,
    Listener::FrequencyOnce
  );

  return true;
}

bool HeartbeatPlugin::shutdown()
{
  W_PRV(HeartbeatPlugin);
  if (d->module)
  {
    wdebug("Killing module since Tunnel wasn't stopped yet.");
    d->shutDownModuleListener->invoke(nullptr);
  }

  Tunnel::instance()->removeEventListener(d->shutDownModuleListener);
  Tunnel::instance()->removeEventListener(d->startUpModuleListener);

  return !(d->module);
}

Plugin::PluginType HeartbeatPlugin::type() const
{
  return Wintermute::Plugin::PluginTypeSupport;
}

W_DECL_PLUGIN(Wintermute::Heartbeat::Plugin, WINTERMUTE_HEARTBEAT_VERSION)
