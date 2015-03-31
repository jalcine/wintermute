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
#include "ping_module.hpp"
#include "globals.hpp"
#include "messages.hpp"

using Wintermute::Heartbeat::PingModule;
using Wintermute::Heartbeat::PongMessage;
using Wintermute::Module;
using Wintermute::Events::Timer;
using Wintermute::Events::Event;
using std::placeholders::_1;

PingModule::PingModule() :
  Module(Wintermute::Module::Designation(
    WINTERMUTE_HEARTBEAT_MODULE_PINGER,
    WINTERMUTE_HEARTBEAT_DOMAIN
  )),
  timer(make_shared<Timer>())
{
  auto bindFunc = bind(&PingModule::onTimerElasped, *this, _1);
  timer->setInterval(WINTERMUTE_HEARTBEAT_INTERVAL);
  timer->listenForEvent(W_EVENT_TIMEOUT, bindFunc);

  listenForEvent(WINTERMUTE_EVENT_MODULE_ENABLE,
    [this](const Event::Ptr& event)
  {
    this->timer->start();
  });
}

PingModule::~PingModule()
{
}

void PingModule::onTimerElasped(const Event::Ptr& event)
{
  assert(event);
  wdebug("Timer elasped; sending ping message.");
  const auto msg = PingMessage::craft();
  sendMessage(msg);
  wdebug("Sent ping message.");
}

bool PingModule::sendMessage(const Message& message)
{
  return Module::sendMessage(message);
}

bool PingModule::receiveMessage(const Message& message) const
{
  return Module::receiveMessage(message);
}
