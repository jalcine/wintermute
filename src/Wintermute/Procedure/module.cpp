/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2014 Jacky Alciné <me@jalcine.me>
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
#include "Wintermute/Procedure/module.hpp"
#include "Wintermute/Procedure/method_call.hpp"
#include "Wintermute/private/Procedure/module.hpp"

using Wintermute::Procedure::Module;
using Wintermute::Procedure::ModulePrivate;

QMap<QString, QPointer<Module>> ModulePrivate::modules =
                               QMap<QString, QPointer<Module>>();

Module::Module ( QObject *parent ) :
  QObject ( parent ), d_ptr ( new ModulePrivate ( this ) )
{
}

QString
Module::domain() const
{
  Q_D ( const Module );
  return d->domain;
}

QString
Module::package() const
{
  Q_D ( const Module );
  return d->package;
}

QString
Module::qualifiedName() const
{
  Q_ASSERT ( !domain().isEmpty() );
  Q_ASSERT ( !package().isEmpty() );
  return domain() + "." + package();
}

QStringList
Module::calls () const
{
  Q_D ( const Module );
  return d->calls.keys();
}

void
Module::invokeCall ( const MethodCall &methodCall )
{
  Q_ASSERT ( methodCall.isValid() );
  Q_D ( Module );
  QVariant result;
  const QString callName = methodCall.name();
  if ( !d->calls.contains ( callName ) ) {
    werr ( this, QString ( "The call '%1' doesn't exist in the module '%2'." )
           .arg ( callName , qualifiedName() ) );
    return;
  }
  QPointer<ModuleCall> moduleCall = d->calls[callName];
  moduleCall->invoke ( methodCall.arguments(), methodCall );
}

QPointer<Module>
Module::findModule ( const QString &name )
{
  Q_ASSERT ( !name.isEmpty() );
  Q_ASSERT ( !name.isNull() );
  return ModulePrivate::modules[name];
}

void
Module::mountCall ( ModuleCall *call )
{
  Q_D ( Module );
  Q_ASSERT ( call != nullptr );
  d->calls.insert ( call->name(), QPointer<ModuleCall>(call) );
}

QList<QPointer<Module>>
                     Module::knownModules()
{
  return ModulePrivate::modules.values();
}

void
Module::setDomain ( const QString &value )
{
  Q_ASSERT ( !value.isEmpty() );
  Q_ASSERT ( !value.isNull() );
  Q_D ( Module );
  d->domain = value;
  d->checkQualifiedName();
}

void
Module::setPackage ( const QString &value )
{
  Q_ASSERT ( !value.isEmpty() );
  Q_ASSERT ( !value.isNull() );
  Q_D ( Module );
  d->package = value;
  d->checkQualifiedName();
}

Module::~Module()
{
  Q_D ( Module );
  for (QPointer<ModuleCall> call : d->calls) {
    Q_ASSERT ( !call.isNull() );
    Q_ASSERT ( call->isValid() );
    call->deleteLater();
  }
}
