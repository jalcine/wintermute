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

#include <wintermutecore/globals.hpp>
#include <wintermutecore/logging.hpp>
#include <wintermutecore/plugin.hpp>
#include "plugin.hh"

using Wintermute::Plugin;
using HeartbeatPluginPrivate = Wintermute::Heartbeat::PluginPrivate;
using std::to_string;

HeartbeatPluginPrivate::PluginPrivate() :
  timer(make_shared<Wintermute::Events::Timer>())
  //module(make_shared<Wintermute::Heartbeat::Module>())
{
  timer->setInterval(WINTERMUTE_HEARTBEAT_INTERVAL);
}

HeartbeatPluginPrivate::~PluginPrivate()
{
}
