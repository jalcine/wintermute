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

#include "Wintermute/private/Procedure/message.hpp"
#include "Wintermute/Procedure/method_call.hpp"
#include "Wintermute/Procedure/reply_call.hpp"

using Wintermute::Procedure::Call;
using Wintermute::Procedure::MethodCall;
using Wintermute::Procedure::ReplyCall;

MethodCall::MethodCall(const QString& name, const Module::Definition& receiver,
                       const QVariant& arguments) : Call(name), m_lambda(nullptr)
{
  setReceiver(receiver);
  QVariantMap aD = callData();
  aD.insert("arguments",arguments);
  setCallData(aD);
}

bool
MethodCall::valid() const
{
  Q_ASSERT ( Call::valid() );
  Q_ASSERT ( callData().contains("arguments") );

  if ( !Call::valid() ) {
    return false;
  }

  if ( !callData().contains("arguments") ) {
    return false;
  }

  return true;
}

QVariant
MethodCall::arguments() const
{
  Q_ASSERT ( valid() );
  QVariant data = callData().value("arguments");
  Q_ASSERT ( data.isValid() );
  Q_ASSERT ( !data.isNull() );
  return data;
}

ReplyCall
MethodCall::craftReply(const QVariant& reply) const
{
  ReplyCall replyCall(*this, reply);
  return replyCall;
}

void
MethodCall::setCallback(const CallbackSignature& lambda)
{
  Q_ASSERT( !lambda == false );
  m_lambda = lambda;
}

MethodCall::~MethodCall()
{
}
