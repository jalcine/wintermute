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
#include "Wintermute/Procedure/method_call.hpp"

using Wintermute::Procedure::Call;
using Wintermute::Procedure::MethodCall;

MethodCall::MethodCall(const QString &methodName, const QString &remoteModule,
                       const QVariant &arguments, const quint64 &pid) : Call(methodName)
{
  QVariantMap receiverMap;
  receiverMap.insert("pid", pid);
  receiverMap.insert("module", remoteModule);
  setReceiver(receiverMap);
  d->dataMap["call"].toMap().insert("arguments", arguments);
}

bool
MethodCall::valid() const
{
  if ( !Call::valid() ) {
    return false;
  }
  if ( !callData().contains("arguments") ) {
    return false;
  }
  if ( d->dataMap["reciever"].type() != QVariant::Map ) {
    return false;
  }
  return true;
}

QVariant
MethodCall::arguments() const
{
  Q_ASSERT ( valid() );
  QVariant data = callData().value("key");
  Q_ASSERT ( data.isValid() );
  Q_ASSERT ( !data.isNull() );
  return data;
}

MethodCall::~MethodCall()
{
}
