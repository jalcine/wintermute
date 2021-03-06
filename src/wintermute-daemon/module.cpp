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

#include <wintermute-core/globals.hpp>
#include <wintermute-core/method.hpp>
#include <wintermute-core/logging.hpp>
#include "module.hpp"
#include "module.hh"

using Wintermute::Method;
using Wintermute::Module;
using DaemonModule = Wintermute::Daemon::Module;
using Wintermute::Events::SignalEvent;

DaemonModule::Module() :
  Wintermute::Module(Module::Designation("in.wintermute", "daemon")),
  d_ptr(new DaemonModule::_Prv)
{
  W_PRV(Daemon::Module);

  d->signalHandler->listenForEvent(W_EVENT_SIGNAL,
  [&](const SignalEvent::Ptr& sigEvent){
    wdebug("Signal given...");
    assert(sigEvent);
  });
}

DaemonModule::~Module()
{
}

void DaemonModule::startRelay()
{
  // TODO: Update 'Module' to have a generator method for this.
  Method::Ptr startMethodCall = std::make_shared<Method>(
    "startRelay",
    Module::Designation(WINTERMUTE_DOMAIN, "heartbeat"),
    designation()
  );

  startMethodCall->invoke();
}

void DaemonModule::stopRelay()
{
  // TODO: Update 'Module' to have a generator method for this.
  Method::Ptr stopMethodCall = std::make_shared<Method>(
    "stopRelay",
    Module::Designation(WINTERMUTE_DOMAIN, "heartbeat"),
    designation()
  );

  stopMethodCall->invoke();
}
