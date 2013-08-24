/**
 * vim: ft=qt.cpp
 * Copyright (C) 2013 Jacky Alcine <me@jalcine.me>
 *
 * This file is part of Wintermute, the extensible AI platform.
 *
 *
 * Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
**/

#include "Wintermute/Procedure/process_module.hpp"
#include "Wintermute/private/Procedure/module.hpp"
#include "Wintermute/Procedure/process_module.moc"

using Wintermute::Procedure::ProcessModule;

ProcessModule::ProcessModule() : Module ( Wintermute::Application::instance() )
{
  setDomain ( "me.jalcine.wintermute" );
  setPackage ( "process" );
  // TODO: Add 'listModules' method via lambda.
  // TODO: Add 'reboot' method via lambda.
}

void
ProcessModule::reboot()
{
  // TODO: Add forking logic or recycle some.
  //Wintermute::Application::instance()->exit();
}

void
ProcessModule::quit ( const int exitcode )
{
  //Wintermute::Application::instance()->exit(exitcode);
}

ProcessModule::~ProcessModule()
{
}
