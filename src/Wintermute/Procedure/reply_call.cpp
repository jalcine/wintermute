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

ReplyCall::ReplyCall( const Call::Pointer &call, const QVariant& response ) :
  Call ( *call ), m_response ( response )
{
  setRecipient ( call->recipient() );
  QVariantMap calleeMap;
  d->type = Call::TypeReply;
  d->data["reply"] = response;
  d->data["call"]  = call->id();
  CallPrivate::calls.take ( id() );
}

Call::Pointer
ReplyCall::call() const
{
  return Call::Pointer(qobject_cast<Call*>(parent()));
}

bool
ReplyCall::isValid() const
{
  Q_ASSERT ( Call::isValid() == true );
  Q_ASSERT ( d->data.contains("reply") == true );
  Q_ASSERT ( d->data.contains("call") == true );
  Q_ASSERT ( wCallCheckFlag ( *this, Call::TypeReply ) == true );

  if ( !Call::isValid() ) return false;
  if ( !d->data.contains ( "reply" ) ) return false;
  if ( !d->data.contains ( "call" ) ) return false;
  if ( !wCallCheckFlag ( *this, Call::TypeReply) ) return false;

  return true;
}

QVariant
ReplyCall::response() const
{
  return m_response;
}

ReplyCall::~ReplyCall()
{
}
