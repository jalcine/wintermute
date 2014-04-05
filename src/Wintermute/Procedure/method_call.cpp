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
#include "Wintermute/private/Procedure/call.hpp"
#include "Wintermute/Procedure/module.hpp"
#include "Wintermute/Procedure/method_call.hpp"
#include "Wintermute/Procedure/reply_call.hpp"
#include "Wintermute/Procedure/dispatcher.hpp"

using Wintermute::Procedure::Call;
using Wintermute::Procedure::Dispatcher;
using Wintermute::Procedure::MethodCall;
using Wintermute::Procedure::ReplyCall;
using Wintermute::Procedure::CallPrivate;

MethodCall::MethodCall ( const QString &module,
                         const QString &method,
                         const QVariantList arguments ) :
  Call ( wntrApp->module() )
{
  setRecipient ( module );
  setSender ( wntrApp->module() );
  d->type = Call::TypeInvocation;
  d->data["method"] = method;
  d->data["arguments"] = arguments;
}

void
MethodCall::setArguments ( const QVariantList &arguments)
{
  d->data.insert ( "arguments", arguments );
}

void
MethodCall::setMethod ( const QString &method )
{
  d->data.insert ( "method", method );
}

void
MethodCall::setModule ( const QString &module )
{
  d->data.insert ( "module", module );
}

QVariantList
MethodCall::arguments() const
{
  return d->data.value ( "arguments" ).toList();
}

QString
MethodCall::method() const
{
  return d->data.value ( "method" ).toString();
}

QString
MethodCall::module() const
{
  return d->data.value ( "module" ).toString();
}

void
MethodCall::setSender ( const Module *module )
{
  QVariantMap appData;
  appData["pid"]     = QCoreApplication::applicationPid();
  appData["version"] = QCoreApplication::applicationVersion();
  appData["module"]  = module->qualifiedName();
  d->data["sender"] = appData;
}

bool
MethodCall::isValid() const
{
  if ( !Call::isValid() ) {
    return false;
  }
  const QVariant value = d->data["sender"];
  const QVariantMap appData = value.toMap();
  Q_ASSERT ( value.isNull() == false );
  Q_ASSERT ( appData.contains("pid") == true );
  Q_ASSERT ( appData.contains("version") == true );
  Q_ASSERT ( appData.contains("module") == true );
  if ( value.isNull() ) {
    return false;
  }
  if ( !appData.contains("pid") ) {
    return false;
  }
  if ( !appData.contains("version") ) {
    return false;
  }
  if ( !appData.contains("module") ) {
    return false;
  }
  return true;
}

void
MethodCall::invoke ( ) const
{
  Q_ASSERT ( isValid() );
  QPointer<Module> theModule = Module::findModule ( recipient() );
  if ( !theModule.isNull() ) {
    werr ( staticMetaObject.className(),
           QString ( "Can't find module '%1' in this process." )
           .arg ( recipient() ) );
  } else {
    theModule->invokeCall ( *this );
  }
}

QPointer<ReplyCall>
MethodCall::craftReply( const QVariant &value ) const
{
  return QPointer<ReplyCall>( new ReplyCall ( *this, value ) );
}

void
MethodCall::dispatch() const
{
  QPointer<Module> const sendingModule =
    Module::findModule(d->data["sender"].toMap()["module"].toString());
  Dispatcher::postDispatch(*this, sendingModule.data());
}

MethodCall::~MethodCall()
{
}
