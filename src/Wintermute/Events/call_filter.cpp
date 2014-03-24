/**
 * Copyright (C) 2011 - 2014 Jacky Alciné <me@jalcine.me>
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
#include "Wintermute/application.hpp"
#include "Wintermute/logging.hpp"
#include "Wintermute/Events/call.hpp"
#include "Wintermute/Events/call_filter.hpp"
#include "Wintermute/Procedure/method_call.hpp"
#include "Wintermute/Procedure/reply_call.hpp"
#include "Wintermute/private/Procedure/call.hpp"
#include "Wintermute/private/Procedure/dispatcher.hpp"

using Wintermute::Events::CallEvent;
using Wintermute::Events::CallFilter;
using Wintermute::Procedure::Call;
using Wintermute::Procedure::CallPrivate;
using Wintermute::Procedure::MethodCall;
using Wintermute::Procedure::ReplyCall;

CallFilter::CallFilter() :
  QObject ( wntrApp )
{
  winfo ( this, "Call filter installed into process." );
}

bool
CallFilter::handleDispatch ( QObject* object, CallEvent* callEvent )
{
  Call* call = const_cast<Call*>(callEvent->call());
  Q_ASSERT ( call->isValid() );
  call->d_ptr->type = call->type() | Call::TypeDispatch;
  Procedure::DispatcherPrivate::dispatch ( call );
  return true;
}

bool
CallFilter::handleReceive ( QObject* object, CallEvent* callEvent )
{
  QVariant callResult;
  Call* call = const_cast<Call*>(callEvent->call());
  Q_ASSERT ( call->isValid() );

  if ( call->type().testFlag ( Call::TypeInvocation ) )
  {
    const MethodCall* methodCall = dynamic_cast<const MethodCall*>(call);
    callResult = Procedure::Call::attemptInvocation ( methodCall );
    ReplyCall* reply = new ReplyCall ( methodCall, callResult );
    CallEvent* event = new CallEvent ( CallEvent::TypeDispatch, reply );
    QCoreApplication::postEvent ( reply, event );
  }
  else if ( call->type().testFlag ( Call::TypeReply ) )
  {
    CallEvent* event = new CallEvent ( CallEvent::TypeReply, call );
    QCoreApplication::postEvent ( object, event ); 
  }
  else
  {
    wwarn ( wntrApp, "Don't know how to handle a foreign call type." );
  }

  return true;
}

bool
CallFilter::handleReply ( QObject* object, CallEvent* callEvent )
{
  Call* call = const_cast<Call*>(callEvent->call());
  Q_ASSERT ( call->isValid() );
  const ReplyCall* replyCall = qobject_cast<const ReplyCall*> ( call );
  Q_ASSERT ( replyCall->isValid() );
  replyCall->call()->handleReply ( replyCall );
  return true;
}

bool
CallFilter::eventFilter ( QObject* object, QEvent* event )
{
  Q_CHECK_PTR ( event );

  if (typeid(*event) == typeid(CallEvent))
  {
    CallEvent* callEvent = (CallEvent*) event;
    if ( event->type() == CallEvent::TypeDispatch )
    {
      return handleDispatch ( object, callEvent );
    }
    else if ( event->type() == CallEvent::TypeReceive )
    { 
      return handleReceive ( object, callEvent );
    }
    else if ( event->type() == CallEvent::TypeReply )
    {
      return handleReply ( object, callEvent );
    }
  }

  return QObject::eventFilter ( object, event );
}

CallFilter::~CallFilter()
{
  winfo ( this, "Call filter removed from process." );
}
