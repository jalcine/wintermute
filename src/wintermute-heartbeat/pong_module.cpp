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
#include <wintermutecore/util.hh>
#include "pong_module.hpp"

using Wintermute::Heartbeat::PongModule;
using Wintermute::Module;
using Wintermute::Util::generate_uuid;

PongModule::PongModule() :
  Module(Wintermute::Module::Designation(
    WINTERMUTE_HEARTBEAT_MODULE_PONGER,
    WINTERMUTE_HEARTBEAT_DOMAIN
  )), uuid()
{
  regenerateUuid();
}

PongModule::~PongModule()
{
}

void PongModule::regenerateUuid()
{
  uuid = generate_uuid();
  wdebug("The UUID of this instance is '" + uuid + "'.");
  assert(!uuid.empty());
}

bool PongModule::sendMessage(const Message& message)
{
  return Module::sendMessage(message);
}

bool PongModule::receiveMessage(const Message& message) const
{
  return Module::receiveMessage(message);
}
