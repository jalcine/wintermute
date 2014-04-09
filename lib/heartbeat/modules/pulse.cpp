/**
 * @author Jacky Alciné <me@jalcine.me>
 * @copyright © 2011, 2012, 2013, 2014 Jacky Alciné <me@jalcine.me>
 * @if 0
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
 * @endif
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

PulseModule::PulseModule( Heartbeat::Plugin* plugin ) :
  Module ( plugin ), d_ptr ( new PulseModulePrivate(this) )
{
  Q_D ( PulseModule );
  setDefinition(WINTERMUTE_HEARTBEAT_DOMAIN, "pulse");
  d->mountCalls();
  wtrace (this, "Ready to beat.");
}

void
PulseModule::start()
{
  Q_D ( PulseModule );
  wtrace(this, "Started.");
  d->timer.start();
}

void
PulseModule::stop()
{
  Q_D ( PulseModule );
  wtrace(this, "Stopped.");
  d->timer.stop();
}

void
PulseModule::tick()
{
  Q_D ( PulseModule );
  pulse ( PulseModule::PulseAlive );
  const Plugin* plugin = qobject_cast<Plugin*>( parent() );
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
  Module::Definition monitorDef = Module::Definition::compose(
                                    WINTERMUTE_HEARTBEAT_DOMAIN, "monitor", 0);
  MethodCall theCall("record", monitorDef, QVariantList() << d->count++ << type );
  theCall.setSender(definition());
  theCall.setCallback ( [&] ( QVariant result ) -> void {
    // @TODO Get the ping stashed as properly recieved (record into monitor?)
    winfo ( this, QString("IS IT REAL?") + result.toString() );
  } );
  theCall.queueForDispatch();
}

PulseModule::~PulseModule()
{
  wtrace ( this, "Pulse giving its last beat." );
}
