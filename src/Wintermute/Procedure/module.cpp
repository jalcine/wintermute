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

#include "Wintermute/logging.hpp"
#include "Wintermute/Procedure/module.hpp"
#include "Wintermute/private/Procedure/module.hpp"
#include "Wintermute/Procedure/lambda_call.hpp"
#include "Wintermute/Procedure/module.moc"

using Wintermute::Procedure::Module;
using Wintermute::Procedure::LambdaCall;
using Wintermute::Procedure::ModulePrivate;

Module::Module ( QObject* parent ) :
  QObject ( parent ), d_ptr ( new ModulePrivate ( this ) )
{
}

const QVariant
Module::dispatch ( const Call& call ) const
{
  Q_D ( const Module );
  const QString callStr = call.toString();
  winfo ( this, QString ( "Sending '%1' to '%2'..." ).arg ( callStr, call.recipient() ) );
  d->sendData ( callStr );
  return QVariant();
}

QVariant
Module::invoke ( const QString callName, const QVariantList data )
{
  Q_D ( Module );
  if ( !d->calls.contains ( callName ) )
  {
    werr ( this, QString ( "The call '%1' doesn't exist in the module '%2'." )
        .arg ( callName, qualifiedName() ) );
    return QVariant ( -1 );
  }
  CallPointer call = d->calls[callName];
  return call->invoke ( data );
}

void
Module::mount ( CallPointer call )
{
  Q_D ( Module );
  // TODO: Use a shared pointer to prevent a segfault.
  d->calls[call->name()].swap ( call );
}

LambdaCall*
Module::mountLambda ( Call::Signature lambda, const QString& name )
{
  CallPointer call = CallPointer ( new LambdaCall ( lambda, name ) );
  this->mount ( call );
  return dynamic_cast<LambdaCall*> ( call.data() );
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
  return domain() + "." + package();
}

void
Module::setDomain ( const QString& value )
{
  Q_D ( Module );
  d->domain = value;
  d->checkQualifiedName();
}

void
Module::setPackage ( const QString& value )
{
  Q_D ( Module );
  d->package = value;
  d->checkQualifiedName();
}

Module::~Module()
{
  // TODO: Report to world that you're leaving us.
}
