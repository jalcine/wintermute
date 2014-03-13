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

#include <QtCore/QCoreApplication>
#include <Wintermute/Globals>
#include "Wintermute/logging.hpp"
#include "Wintermute/Procedure/process_module.hpp"
#include "Wintermute/Procedure/process_module.moc"

using Wintermute::Procedure::ProcessModule;

ProcessModule::ProcessModule() : Module ( Wintermute::Application::instance() )
{
  setDomain  ( WINTERMUTE_DOMAIN );
  setPackage ( "process" );
}

void
ProcessModule::start()
{
  connect ( wntrApp, SIGNAL ( started() ), SLOT ( start() ) );
  winfo ( this, QString ( "Currently %1 modules loaded so far." )
      .arg ( wntrApp->modules().length() ) );
  // TODO: Move the mode handling logic here?
}

void
ProcessModule::reboot()
{
  Wintermute::Application::instance()->stop();
}

void
ProcessModule::stop()
{
  winfo ( this, QString ("Stopping Wintermute...") );
  quit ();
}

void
ProcessModule::quit ( const int exitcode )
{
  winfo ( this, "Invoking remote exit..." );
  wntrApp->stop ( exitcode );
}

ProcessModule::~ProcessModule()
{
  if ( !QCoreApplication::closingDown() )
    stop();
}
