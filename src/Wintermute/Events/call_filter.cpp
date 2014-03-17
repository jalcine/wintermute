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

#include "Wintermute/application.hpp"
#include "Wintermute/logging.hpp"
#include "Wintermute/Events/call.hpp"
#include "Wintermute/Events/call_filter.hpp"
#include "Wintermute/Procedure/method_call.hpp"
#include "Wintermute/Procedure/reply_call.hpp"
#include "Wintermute/private/Procedure/dispatcher.hpp"
#include "Wintermute/private/Procedure/call.hpp"

using Wintermute::Events::CallEvent;
using Wintermute::Events::CallFilter;
using Wintermute::Procedure::Call;
using Wintermute::Procedure::CallPrivate;
using Wintermute::Procedure::ReplyCall;

CallFilter::CallFilter() :
  QObject ( wntrApp )
{
  winfo ( this, "Call filter installled into process." );
}

bool
CallFilter::handleDispatch ( QObject* object, QEvent* event )
{
  CallEvent* callEvent = dynamic_cast<CallEvent*> ( event );
  const Call* call = callEvent->call();

  if ( call->isValid() && call->type() == Call::TypeInvocation )
  {
    Procedure::DispatcherPrivate::dispatch ( call->toString() );
  }

  return true;
}

bool
CallFilter::handleReceive ( QObject* object, QEvent* event )
{
  CallEvent* callEvent = dynamic_cast<CallEvent*> ( event );
  const Call* call = callEvent->call();
  const QVariant callResult = Procedure::Call::attemptInvocation ( call );

  if ( call->isValid() )
  {
    ReplyCall reply ( call, callResult );
    Procedure::Module* module = wntrApp->module();
    module->dispatch ( reply );
  }

  return true;
}

bool
CallFilter::handleReply ( QObject* object, QEvent* event )
{
  CallEvent* callEvent;
  const ReplyCall* replyCall;
  const Call* baseCall;

  callEvent = dynamic_cast<CallEvent*> ( event );
  replyCall = qobject_cast<const ReplyCall*>( callEvent->call() );
  const quint16 id = replyCall->call()->d_ptr->data[ "id" ].toInt();
  baseCall = CallPrivate::calls [ id ];

  if ( replyCall->isValid () )
  {
    baseCall->handleReply ( replyCall );
  }

  return true;
}

bool
CallFilter::eventFilter ( QObject* object, QEvent* event )
{
  if ( event->type() == CallEvent::TypeDispatch )
  {
    return handleDispatch ( object, event );
  }
  else if ( event->type() == CallEvent::TypeReceive )
  { 
    return handleReceive ( object, event );
  }
  else if ( event->type() == CallEvent::TypeReply )
  {
    return handleReply ( object, event );
  }

  return QObject::eventFilter ( object, event );
}

CallFilter::~CallFilter()
{
  winfo ( this, "Call filter removed from process." );
}
