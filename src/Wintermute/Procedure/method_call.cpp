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

using Wintermute::Procedure::MethodCall;
using Wintermute::Procedure::Call;

MethodCall::MethodCall ( const QString& module, 
    const QString& method,
    const QVariantList arguments ) :
  Call ( new MethodCallPrivate ( this ) )
{
  Q_D ( MethodCall );
  setRecipient ( module );
  d->data["method"] = method;
  d->data["arguments"] = arguments;
}

void
MethodCall::setArguments ( const QVariantList& arguments)
{
  Q_D ( MethodCall );
  d->data.insert ( "arguments", arguments );
}

void
MethodCall::setMethod ( const QString& method )
{
  Q_D ( MethodCall );
  d->data.insert ( "method", method );
}

void
MethodCall::setModule ( const QString& module )
{
  Q_D ( MethodCall );
  d->data.insert ( "module", module );
}

QVariantList
MethodCall::arguments() const
{
  Q_D ( const MethodCall );
  return d->data.value ( "arguments" ).toList();
}

QString
MethodCall::method() const
{
  Q_D ( const MethodCall );
  return d->data.value ( "method" ).toString();
}

QString
MethodCall::module() const
{
  Q_D ( const MethodCall );
  return d->data.value ( "module" ).toString();
}

void
MethodCall::dispatch ( Module* module )
{
  Q_D ( MethodCall );
  d->composeMethodData( module );
  module->dispatch ( this );
}

bool
MethodCall::isValid() const
{
  Q_D ( const MethodCall );
  return Call::isValid() && d->isValid();
}

MethodCall::~MethodCall()
{
}
