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
#include "Wintermute/Events/Filters/call.hpp"
#include "Wintermute/Procedure/method_call.hpp"
#include "Wintermute/private/Procedure/dispatcher.hpp"
#include "Wintermute/Events/Filters/call.moc"

using Wintermute::Events::CallEvent;
using Wintermute::Events::Filters::CallFilter;
using Wintermute::Procedure::Call;

CallFilter::CallFilter() :
  QObject ( wntrApp )
{
  winfo ( this, "Call filter installled into process." );
}

bool
CallFilter::handleDispatch ( QObject* object, QEvent* event )
{
  winfo ( this, "Handling a local call for dispatching aboard." );

  CallEvent* callEvent = static_cast<CallEvent*> ( event );
  const Procedure::Call* call = callEvent->call();

  winfo ( this, QString ( "Dispatching %1 to '%2'." )
    .arg ( call->toString(), call->recipient() ) );

  Procedure::DispatcherPrivate::dispatch ( call->toString() );

  winfo ( this, "Call dispatched." );

  return true;
}

bool
CallFilter::handleReceive ( QObject* object, QEvent* event )
{
  winfo ( this, "Handling a remote call for local invocation." );

  CallEvent* callEvent = static_cast<CallEvent*> ( event );
  const Procedure::Call* call = callEvent->call();
  const bool invocated = Procedure::Call::attemptInvocation ( call );

  invocated ? winfo ( this, "Call invoked." ) :
    wwarn ( this, "Call failed to invoke." );

  return true;
}

bool
CallFilter::eventFilter ( QObject* object, QEvent* event )
{
  if ( event->type() == CallEvent::TypeDispatch )
  {
    return handleDispatch( object, event );
  }
  else if ( event->type() == CallEvent::TypeReceive )
  { 
    return handleReceive( object, event );
  }

  return QObject::eventFilter ( object, event );
}

CallFilter::~CallFilter()
{
  winfo ( this, "Call filter removed from process." );
}
