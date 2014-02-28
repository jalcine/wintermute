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
#include "Wintermute/Procedure/module.hpp"
#include "Wintermute/Procedure/method_call.hpp"
#include "Wintermute/private/Procedure/method_call.hpp"
#include "Wintermute/Procedure/method_call.moc"

using Wintermute::Procedure::MethodCall;
using Wintermute::Procedure::Call;

MethodCall::MethodCall ( const QString& module, const QString& method, const QVariantList arguments ) :
  Call ( new MethodCallPrivate ( this ) )
{
  Q_D ( MethodCall );
  setRecipient ( module );
  d->type = Call::TypeInvocation;
  d->data["method"] = method;
  d->data["arguments"] = arguments;
}

QVariantList
MethodCall::arguments() const
{
  Q_D ( const MethodCall );
  return d->data.value ( "arguments" ).toList();
}

void
MethodCall::dispatch ( Module* module )
{
  Q_D ( MethodCall );
  QMap<QString, QVariant> appData;
  appData["pid"]     = QCoreApplication::applicationPid();
  appData["version"] = QCoreApplication::applicationVersion();
  appData["module"]  = module->qualifiedName();
  d->data["sender"]  = appData;
  module->dispatch ( *this );
}

MethodCall::~MethodCall()
{
}
