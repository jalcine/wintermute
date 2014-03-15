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
#include <Wintermute/Procedure/MethodCall>
#include "plugin.hpp"
#include "pulse.hpp"
#include "private/modules/pulse.hpp"

using Wintermute::Procedure::MethodCall;
using Wintermute::Heartbeat::Plugin;
using Wintermute::Heartbeat::PulseModule;
using Wintermute::Heartbeat::PulseModulePrivate;

PulseModule::PulseModule( Heartbeat::Plugin* plugin ) :
  Module ( plugin ), d_ptr ( new PulseModulePrivate(this) )
{
  winfo (this, "Pulse ready to beat.");
  setDomain ( WINTERMUTE_HEARTBEAT_DOMAIN );
  setPackage ( "pulse" );

  mountLambda ( "module", [&] (QVariantList args) -> QVariant {
    Q_D ( PulseModule );
    const QString moduleName = args[0].toString();
    const QVariant module = d->getModuleInfo ( moduleName );
    wdebug ( this, module.toString() );
    return module;
  } );

  mountLambda ( "modules", [&] (QVariantList args) -> QVariant {
    Q_D ( PulseModule );
    const QVariant modules = d->getAllModulesInfo();
    wdebug ( this, modules.toString() );
    return modules;
  } );
}

void
PulseModule::start()
{
  Q_D ( PulseModule );
  d->timer.start();
}

void
PulseModule::stop()
{
  Q_D ( PulseModule );
  d->timer.stop();
}

void
PulseModule::tick()
{
  Q_D ( PulseModule );
  pulse (PulseModule::PulseAlive);
  d->timer.start();
}

void
PulseModule::pulse(PulseType type)
{
  Q_D ( PulseModule );
  MethodCall theCall (WINTERMUTE_HEARTBEAT_DOMAIN".monitor", "record");
  quint64 pid = QCoreApplication::applicationPid();
  theCall.setArguments(QVariantList() << d->count++ << type << pid );
  dispatch ( theCall );
}

PulseModule::~PulseModule()
{
  winfo (this, "Pulse giving its last beat.");
}
