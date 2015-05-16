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
#include <wintermute-core/module.hpp>
#include <wintermute-core/logging.hpp>
#include "module.hh"
#include "ping_module.hpp"
#include "pong_module.hpp"
#include "plugin.hpp"

using Wintermute::Heartbeat::ModulePrivate;
using HBModule = Wintermute::Heartbeat::Module;

ModulePrivate::ModulePrivate() :
  modeModule(nullptr),
  mode(HBModule::ModeUndefined)
{
  determineMode();
}

ModulePrivate::~ModulePrivate()
{
  tearDownSubModule();
}

void ModulePrivate::determineMode()
{
  if (mode == HBModule::ModeUndefined)
  {
    const bool hasDaemon =
      Module::Pool::instance()->has(
        Module::Designation("daemon","in.wintermute")
    );

    mode = (hasDaemon) ? HBModule::ModePong : HBModule::ModePing;
  }
  else
  {
    wwarn("Already did detection of mode!");
  }

  wdebug("Determined mode to run heartbeat as " + to_string((int)mode) + ".");
}

void ModulePrivate::setUpSubModule()
{
  wdebug("Setting up mode-specific module for heartbeat...");

  switch (mode)
  {
    case HBModule::ModePing:
      wdebug("This instance will be pinging.");
      modeModule = make_shared<Heartbeat::PingModule>();
      break;
    case HBModule::ModePong:
      wdebug("This instance will be ponging.");
      modeModule = make_shared<Heartbeat::PongModule>();
      break;
    default:
      wwarn("This instance will be doing nothing " + to_string((int)mode) + ".");
      break;
  }

  if (modeModule)
  {
    auto des = modeModule->designation();
    wdebug("Setting up heartbeat sub-module " + static_cast<string>(des) + "..."); 
    modeModule->enable();
    wdebug("Set up heartbeat sub-module " + static_cast<string>(des) + "."); 
  }
}

void ModulePrivate::tearDownSubModule()
{
  if (modeModule)
  {
    auto des = modeModule->designation();
    wdebug("Tearing down heartbeat sub-module " + static_cast<string>(des) + "...");
    modeModule->disable();
    modeModule = nullptr;
    wdebug("Tore down heartbeat sub-module.");
  }
  else
  {
    wdebug("No sub module to tear down.");
  }
}
