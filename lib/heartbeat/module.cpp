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
#include <Wintermute/Globals>
#include <Wintermute/Logging>
#include <Wintermute/Procedure/Call>
#include "plugin.hpp"
#include "module.hpp"
#include "module.moc"

using Wintermute::Heartbeat::Module;
using Wintermute::Heartbeat::Plugin;
using Wintermute::Procedure::Call;
using Wintermute::Procedure::CallPointer;

Module::Module ( Heartbeat::Plugin* plugin ) :
  Wintermute::Procedure::Module ( plugin )
{
  setDomain ( WINTERMUTE_DOMAIN );
  setPackage ( "heartbeat" );
  winfo ( this, "Heartbeat ready to pulse." );
}

// TODO Figure out how to use a bound reference to a method of an object in
//      std::function.
void
Module::start()
{
  winfo ( this, "Started heartbeat module." );
  Call::Signature greet;
  mountLambda ( greet, "greet" );
  winfo ( this, "Mounted calls for heartbeat." );
}

QVariant
Module::callGreet( QVariantList arguments )
{
  winfo ( this, "Hey, look; you're here!" );
}

Module::~Module()
{
}
