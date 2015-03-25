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
#include "pong_module.hpp"
#include <wintermutecore/logging.hpp>

using std::dynamic_pointer_cast;
using Wintermute::Message;
using Wintermute::Heartbeat::Module;
using Wintermute::Heartbeat::PongMessage;

// TODO: Have this require a module to pull the UUID from.
PongMessage::PongMessage() :
  Message()
{
}

PongMessage::~PongMessage()
{
}

/*
 * TODO: Craft the required UUID message for this instance.
 */
Message PongMessage::craft()
{
  Message msg;
  auto hbDes = Module::Designation(
    WINTERMUTE_HEARTBEAT_MODULE_PONGER,
    WINTERMUTE_HEARTBEAT_DOMAIN
  );

  auto modulePtr = Module::Pool::instance()->find(hbDes);
  assert(modulePtr);

  auto pongModulePtr = dynamic_pointer_cast<Heartbeat::PongModule>(modulePtr);
  assert(pongModulePtr);

  if (!modulePtr || !pongModulePtr)
  {
    return msg;
  }

  const string uuid = pongModulePtr->uuid;

  Message::HashType msgData;
  msgData.emplace("uuid", uuid);
  msg.setPayload(msgData);

  return msg;
}
