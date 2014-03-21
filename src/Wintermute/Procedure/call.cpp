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

#include <qjson/serializer.h>
#include <qjson/parser.h>
#include "Wintermute/logging.hpp"
#include "Wintermute/application.hpp"
#include "Wintermute/Procedure/call.hpp"
#include "Wintermute/private/Procedure/call.hpp"
#include "Wintermute/Procedure/reply_call.hpp"
#include "Wintermute/Procedure/module.hpp"

using Wintermute::Procedure::Call;
using Wintermute::Procedure::CallPrivate;
using Wintermute::Procedure::Module;

CallPrivate::CallCache CallPrivate::calls;

Call::Call ( QObject* parent ) :
  QObject ( parent ), d_ptr ( new CallPrivate ( this ) )
{
}

Call::Call ( CallPrivate* old_d ) :
  QObject ( Wintermute::Application::instance() ), d_ptr ( old_d )
{
  Q_D ( Call );
  Q_ASSERT ( old_d );
  d->q_ptr = this;
}

bool
Call::isValid() const
{
  Q_D ( const Call );
  return d->isValid();
}

QVariant
Call::invoke ( const QVariantList& data )
{
  winfo ( this, "This is an error." );
  return QVariant();
}

void
Call::handleReply ( const ReplyCall* reply ) const
{
  Q_D ( const Call );
  if ( d->callback )
  {
    d->callback ( reply->response() );
    winfo ( this, "Invoked callback to reply." );
  }
  else
  {
    winfo ( this, "No callback to invoke." );
  }
}

void
Call::setRecipient ( const QString moduleName )
{
  Q_D ( Call );
  d->recipient = QString ( moduleName );
}

Call::Type
Call::type() const
{
  Q_D ( const Call );
  return d->type;
}

QString
Call::name() const
{
  Q_D ( const Call );
  return d->name;
}

QString
Call::recipient() const
{
  Q_D ( const Call );
  return d->recipient;
}

quint64
Call::id() const
{
  Q_D ( const Call );
  return d->id.toTime_t();
}

QString
Call::toString() const
{
  Q_D ( const Call );
  bool ok;
  QJson::Serializer serializer;
  QVariantMap callData = d->toVariantMap();
  QString json = serializer.serialize ( callData, &ok );
  return ( ok ? json : QString::null );
}

Call*
Call::fromString ( const QString& data )
{
  QJson::Parser parser;
  bool ok;
  QVariantMap callData = parser.parse ( data.toLocal8Bit(), &ok ).toMap();

  if ( !ok ) 
  {
    werr ( wntrApp, QString("Failed to convert '%1' into a Call.").arg(data) );
    return nullptr;
  }

  CallPrivate *d_ptr = CallPrivate::fromVariantMap(callData);
  return ( d_ptr->isValid() ? new Call (d_ptr) : 0 );
}

QVariant
Call::operator() ( const QVariantList& data )
{
  return invoke ( data );
}

QVariant
Call::attemptInvocation ( const Call* call )
{
  Procedure::Module* module = wntrApp->findModule ( call->recipient() );
  if ( !module )
  {
    werr ( staticMetaObject.className(), 
      QString ( "Can't find module '%1' in this process." )
        .arg ( call->recipient() ) );
    return false;
  }

  const QString methodName     = call->d_ptr->data[ "method" ].toString();
  const QVariantList arguments = call->d_ptr->data[ "arguments" ].toList();

  QVariant result = module->invoke ( methodName, arguments );
  return result;
}

void
Call::setCallback ( Call::CallbackSignature callback )
{
  Q_D ( Call );
  d->callback = callback;
}

void
Call::clearCallback ()
{
  Q_D ( Call );
  d->callback = nullptr;
}

Call::~Call()
{
}
