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

#include "Wintermute/private/Procedure/message.hpp"
#include "Wintermute/Procedure/reply_call.hpp"

using Wintermute::Procedure::Call;
using Wintermute::Procedure::ReplyCall;
using Wintermute::Procedure::MethodCall;

ReplyCall::ReplyCall( const MethodCall& call, const QVariant& response ) :
  Call ( call.name() ), m_methodCall ( call )
{
  QVariantMap replyMap;
  replyMap.insert("call", QVariant());
  replyMap.insert("value", response);
  d->dataMap.insert("reply", replyMap);
  setReceiver(call.sendingModule());
}

ReplyCall::ReplyCall( const ReplyCall& other ) : Call ( other ),
  m_methodCall ( other.m_methodCall )
{
}

ReplyCall::ReplyCall( const Call& other ) : Call ( other ),
  m_methodCall( Message() )
{
}

const MethodCall&
ReplyCall::methodCall() const
{
  return m_methodCall;
}

bool
ReplyCall::valid() const
{
  Q_ASSERT ( Call::valid() == true );
  Q_ASSERT ( d->dataMap.contains("reply") == true );

  if ( !Call::valid() ) {
    return false;
  }

  if ( !d->dataMap.contains ( "reply" ) ) {
    return false;
  }

  QVariant replyVariant = d->dataMap.value("reply");

  if ( !replyVariant.isValid() ) {
    return false;
  }

  if ( replyVariant.isNull() ) {
    return false;
  }

  QVariantMap replyMap = replyVariant.toMap();

  if ( !replyMap.contains("call") ) {
    return false;
  }

  if ( !replyMap.contains("value") ) {
    return false;
  }

  return true;
}

QVariant
ReplyCall::response() const
{
  Q_ASSERT ( valid() );
  QVariant replyVariant = d->dataMap.value("reply");

  if ( replyVariant.isValid() ) {
    return replyVariant.toMap().value("value");
  }

  return QVariant();
}

ReplyCall::~ReplyCall()
{
}
