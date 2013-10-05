/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2013 Jacky Alciné <me@jalcine.me>
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

#include "Wintermute/logging.hpp"
#include "Wintermute/Procedure/process_module.hpp"
#include "Wintermute/Procedure/process_module.moc"

using Wintermute::Procedure::ProcessModule;

ProcessModule::ProcessModule() : Module ( Wintermute::Application::instance() )
{
  setDomain ( "me.jalcine.wintermute" );
  setPackage ( "process" );
  connect(wntrApp, SIGNAL(started()), SLOT(start()));
}

void
ProcessModule::start()
{
  // TODO: What?
  winfo(this, QString("Currently %1 modules loaded so far.").arg(wntrApp->modules().length()));
}

void
ProcessModule::reboot()
{
  // TODO: Add forking logic or recycle some.
  Wintermute::Application::instance()->stop();
}

void
ProcessModule::quit ( const int exitcode )
{
  winfo ( this, "Invoking remote exit..." );
  wntrApp->stop ( exitcode );
}

ProcessModule::~ProcessModule()
{
}
