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

#include "messages.hpp"
#include <wintermutecore/module.hpp>
#include <wintermutecore/plugin.hpp>
#include <wintermutecore/logging.hpp>
#include <wintermutecore/util.hh>

using Wintermute::Message;
using Wintermute::Plugin;
using Wintermute::Heartbeat::PingMessage;

PingMessage::PingMessage() : Message()
{
}

PingMessage::~PingMessage()
{
}

Message PingMessage::craft()
{
  auto pluginList = Plugin::all();
  auto moduleList = Module::Pool::instance()->modules();

  list<string> pluginNames, moduleNames;

  for ( auto pluginName : pluginList )
  {
    pluginNames.push_back(pluginName);
  }

  if (!moduleList.empty())
  {
    for ( auto modulePtr : moduleList )
    {
      string name = modulePtr->designation().domain() + "."
        + modulePtr->designation().name();
      moduleNames.push_back(name);
    }
  }

  const string pluginNamesStr = Wintermute::Util::join_string(pluginNames, ",");
  const string moduleNamesStr = Wintermute::Util::join_string(moduleNames, ",");

  Message msg;
  Message::HashType msgData;
  msgData.emplace("modules", moduleNamesStr);
  msgData.emplace("plugins", pluginNamesStr);
  msg.setPayload(msgData);
  return msg;
}
