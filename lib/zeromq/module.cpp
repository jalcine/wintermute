/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
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

#include <Wintermute/Application>
#include <Wintermute/Logging>
#include "module.hpp"
#include "plugin.hpp"
#include "dispatcher.hpp"
#include "module.moc"

using Wintermute::ZeroMQ::Module;
using Wintermute::ZeroMQ::Plugin;
using Wintermute::ZeroMQ::Dispatcher;

Module::Module ( ZeroMQ::Plugin* plugin ) : Wintermute::Procedure::Module ( plugin )
{
  setDomain ( "me.jalcine.wintermute" );
  setPackage ( "zeromq" );
  winfo(this, "Pumping in ZeroMQ goodness to Wintermute...");
  Dispatcher* dispatcher = new Dispatcher;
  dispatcher->setParent(this);
  winfo(this, "Thanks Pete, Wintermute's on-line.");
}

void
Module::start()
{
  // TODO: Spawn out a few plug-ins needed for kicking up Wintermute.
}

Module::~Module()
{
}
