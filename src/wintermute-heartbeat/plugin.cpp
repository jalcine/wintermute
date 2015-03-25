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
#include <wintermutecore/logging.hpp>
#include "plugin.hh"
#include "plugin.hpp"

using Wintermute::Plugin;
using HeartbeatPlugin = Wintermute::Heartbeat::Plugin;
using HeartbeatPluginPrivate = Wintermute::Heartbeat::PluginPrivate;

HeartbeatPlugin::Plugin() :
  Wintermute::Plugin(WINTERMUTE_HEARTBEAT_PLUGIN_NAME),
  d_ptr(new HeartbeatPluginPrivate)
{
}

HeartbeatPlugin::~Plugin()
{
}

bool HeartbeatPlugin::startup()
{
  W_PRV(HeartbeatPlugin);
  d->timer->start();
  return false;
}

bool HeartbeatPlugin::shutdown()
{
  return false;
}

Plugin::PluginType HeartbeatPlugin::type() const
{
  return Wintermute::Plugin::PluginTypeSupport;
}

HeartbeatPlugin::Mode HeartbeatPlugin::mode() const
{
  W_PRV(const HeartbeatPlugin);
  return d->mode;
}

void HeartbeatPlugin::setMode(const Mode modeToUse)
{
  W_PRV(HeartbeatPlugin);
  d->mode = modeToUse;
}

W_DECL_PLUGIN(Wintermute::Heartbeat::Plugin, WINTERMUTE_HEARTBEAT_VERSION)
