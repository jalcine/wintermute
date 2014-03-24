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

#include <QtCore/QVariant>
#include "call.hpp"
#include "Wintermute/private/Procedure/call.hpp"
#include "reply_call.hpp"

using Wintermute::Procedure::Call;
using Wintermute::Procedure::ReplyCall;

ReplyCall::ReplyCall( const Call* call, const QVariant& response ) :
  Call ( const_cast<Call*>(call) ), m_response ( response )
{
  Q_D ( Call );
  setRecipient ( call->recipient() );
  QVariantMap calleeMap;
  d->type = Call::TypeReply;
  d->data["reply"] = response;
  d->data["call"]  = call->id();
  CallPrivate::calls.take ( id() );
  setParent ( wntrApp );
}

Call*
ReplyCall::call() const
{
  return (Call*) parent();
}

bool
ReplyCall::isValid() const
{
  Q_D ( const Call );
  if ( !d->data.contains ( "reply" ) ) return false;
  if ( !d->data.contains ( "call" ) ) return false;
  if ( !d->type != Call::TypeReply ) return false;

  return Call::isValid();
}

QVariant
ReplyCall::response() const
{
  return m_response;
}

ReplyCall::~ReplyCall()
{
}
