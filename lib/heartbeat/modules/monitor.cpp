/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2014 Jacky Alciné <me@jalcine.me>
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

#include "plugin.hpp"
#include "globals.hpp"
#include "monitor.hpp"
#include "private/modules/monitor.hpp"

using Wintermute::Heartbeat::MonitorModule;
using Wintermute::Heartbeat::MonitorModulePrivate;

MonitorModule::MonitorModule( Heartbeat::Plugin* plugin ) :
  Wintermute::Procedure::Module ( plugin ), 
  d_ptr ( new MonitorModulePrivate ( this ) )
{
  Q_D ( MonitorModule );
  winfo (this,
      "Getting ready to listen to the heartbeat pulse.");
  setDomain ( WINTERMUTE_HEARTBEAT_DOMAIN );
  setPackage ( "monitor" );
  d->mountCalls();
}

void
MonitorModule::start()
{
  Q_D ( MonitorModule );
  d->processes.clear();
}

void
MonitorModule::stop()
{
  // TODO: If the monitor goes down; what should we do? Let the kids go play? 
  // NOTE: Consider sending a 'mute' signal to the processes.
}

MonitorModule::~MonitorModule()
{
  winfo ( this, "No more watching the kids." );
}
