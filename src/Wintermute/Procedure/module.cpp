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

#include <QtCore/QMetaType>
#include <QtCore/QStringList>
#include "Wintermute/logging.hpp"
#include "Wintermute/Procedure/module.hpp"
#include "Wintermute/Procedure/method_call.hpp"
#include "Wintermute/private/Procedure/module.hpp"

using Wintermute::Procedure::Module;
using Wintermute::Procedure::ModuleHash;
using Wintermute::Procedure::ModuleList;
using Wintermute::Procedure::ModulePrivate;

ModuleHash ModulePrivate::modules = ModuleHash();



Module::Module ( QObject* parent ) : QObject ( parent ),
  d_ptr ( new ModulePrivate ( this ) )
{
}

Module::Definition
Module::definition() const
{
  Q_D ( const Module );
  Q_ASSERT ( d->definition.valid() );
  return d->definition;
}

QStringList
Module::calls () const
{
  Q_D ( const Module );
  return d->calls.keys();
}

void
Module::invokeCall ( const MethodCall& methodCall )
{
  Q_ASSERT ( methodCall.valid() );
  Q_D ( Module );
  QVariant result;
  const QString callName = methodCall.name();
  const bool hasCall = d->calls.contains(callName);
  Q_ASSERT ( hasCall == true );

  if ( !hasCall ) {
    werr ( this, QString ( "The call '%1' doesn't reside here." ).arg (callName));
    return;
  }

  ModuleCall* moduleCall = d->calls.value(callName);
  wtrace(this, QString("Invoking call %1...").arg(callName));
  moduleCall->invoke ( methodCall.arguments(), methodCall );
}

Module*
Module::findModule ( const Module::Definition& definition )
{
  /// @todo Check if the pids match.
  Q_ASSERT ( definition.valid() );
  return ModulePrivate::modules.value(definition);
}

void
Module::mountCall ( ModuleCall* call )
{
  Q_D ( Module );
  Q_CHECK_PTR ( call );
  wtrace(this, QString("Added call %1.").arg(call->name()));
  d->calls.insert ( call->name(), call );
}

const ModuleList
Module::knownModules()
{
  return ModulePrivate::modules.values();
}

void
Module::setDefinition(const QString& domain, const QString& package,
                      const quint64& pid)
{
  Q_ASSERT ( !domain.isNull() );
  Q_ASSERT ( !domain.isEmpty() );
  Q_ASSERT ( !package.isNull() );
  Q_ASSERT ( !package.isEmpty() );
  Module::setDefinition(Module::Definition::compose(domain,package,pid));
  return;
}

void
Module::setDefinition(const Module::Definition& aDef)
{
  Q_ASSERT ( aDef.valid() );
  Q_D ( Module );
  d->definition = aDef;
}

Module::~Module()
{
}

QDataStream& operator<<(QDataStream& out, const Module::Definition& obj)
{
  out << obj.pid << obj.domain << obj.package;
  return out;
}

QDataStream& operator>>(QDataStream& in, Module::Definition& obj)
{
  in >> obj.pid >> obj.domain >> obj.package;
  return in;
}
