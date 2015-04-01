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

#include "module.hpp"
#include "module.hh"
#include <wintermute-core/logging.hpp>
#include <wintermute-core/util.hh>

using Wintermute::Module;
using HeartbeatModule = Wintermute::Heartbeat::Module;

HeartbeatModule::Module() :
  Wintermute::Module(Wintermute::Module::Designation(
    WINTERMUTE_HEARTBEAT_MODULE,
    WINTERMUTE_HEARTBEAT_DOMAIN
  )),
  d_ptr(make_shared<HeartbeatModule::_Prv>())
{
  W_PRV(Heartbeat::Module);

  auto disableModuleFunc = [this](const Events::Event::Ptr& event)
  {
    wdebug("Heartbeat sub module disabling...");
    this->d_ptr->tearDownSubModule();
    wdebug("Heartbeat sub module disabled.");
  };

  d->setUpSubModule();
  listenForEvent(WINTERMUTE_EVENT_MODULE_DISABLE, disableModuleFunc);
}

HeartbeatModule::~Module()
{
}

bool HeartbeatModule::sendMessage(const Message& message)
{
  return false;
}

bool HeartbeatModule::receiveMessage(const Message& message) const
{
  return false;
}

HeartbeatModule::Mode HeartbeatModule::mode() const
{
  W_PRV(const Heartbeat::Module);
  return d->mode;
}

void HeartbeatModule::setMode(const HeartbeatModule::Mode modeToUse)
{
  W_PRV(Heartbeat::Module);
  d->mode = modeToUse;
}
