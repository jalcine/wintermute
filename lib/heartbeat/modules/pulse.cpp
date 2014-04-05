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

#include <QtCore/QVariantMap>
#include <QtCore/QCoreApplication>
#include <Wintermute/Procedure/MethodCall>
#include "plugin.hpp"
#include "pulse.hpp"
#include "private/modules/pulse.hpp"

using Wintermute::Procedure::Call;
using Wintermute::Procedure::MethodCall;
using Wintermute::Heartbeat::Plugin;
using Wintermute::Heartbeat::PulseModule;
using Wintermute::Heartbeat::PulseModulePrivate;

PulseModule::PulseModule( Heartbeat::Plugin *plugin ) :
  Module ( plugin ), d_ptr ( new PulseModulePrivate(this) )
{
  Q_D ( PulseModule );
  setDomain ( WINTERMUTE_HEARTBEAT_DOMAIN );
  setPackage ( "pulse" );
  d->mountCalls();
  winfo (this, "Ready to beat.");
}

void
PulseModule::start()
{
  Q_D ( PulseModule );
  winfo(this, "Started.");
  d->timer.start();
}

void
PulseModule::stop()
{
  Q_D ( PulseModule );
  winfo(this, "Stopped.");
  d->timer.stop();
}

void
PulseModule::tick()
{
  Q_D ( PulseModule );
  pulse ( PulseModule::PulseAlive );
  const Plugin *plugin = qobject_cast<Plugin *>( parent() );
  const QVariant interval = plugin->configuration()->value("Pulse/Interval");
  if ( interval.isValid() ) {
    d->timer.setInterval ( interval.toInt() );
  }
  d->timer.start();
}

void
PulseModule::pulse( PulseType type )
{
  Q_D ( PulseModule );
  d->timer.stop();
  MethodCall *theCall = new MethodCall( WINTERMUTE_HEARTBEAT_DOMAIN".monitor",
                                        "record");
  quint64 pid = QCoreApplication::applicationPid();
  theCall->setArguments (QVariantList() << d->count++ << type << pid );
  theCall->setSender ( this );
  //theCall->setCallback ( [&] ( QVariant result ) -> void {
  // TODO Get the ping stashed as properly recieved (record into monitor?)
  //winfo ( this, QString("IS IT REAL?") + result.toString() );
  //} );
  winfo ( this, "Sending a pulse..." );
  theCall->dispatch();
}

PulseModule::~PulseModule()
{
  winfo ( this, "Pulse giving its last beat." );
}
