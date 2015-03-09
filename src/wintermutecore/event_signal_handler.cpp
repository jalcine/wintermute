/*
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with Wintermute; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "events.hpp"
#include "event_signal_handler.hh"
#include "logging.hpp"

using Wintermute::Events::Event;
using Wintermute::Events::Emitter;
using Wintermute::Events::SignalEvent;
using Wintermute::Events::SignalHandler;
using Wintermute::Events::SignalHandlerPrivate;

void w_event_signal_cb(uv_signal_t* handle, int signum)
{
  wdebug("Obtained signal event for signal " + to_string(signum) + "...");
  SignalHandlerPrivate* dptr = (SignalHandlerPrivate*)(handle->data);
  assert(dptr);
  Event::Ptr event = make_shared<SignalEvent>(dptr->q_ptr);
  dptr->emitter->emit(event);
}

SignalHandler::SignalHandler(const int& signalNumber) :
  d_ptr(new SignalHandlerPrivate(signalNumber))
{
  W_PRV(SignalHandler);
  d->q_ptr = make_shared<SignalHandler>(*this);
  d->attachSignal();
}

SignalHandler::~SignalHandler()
{
}

Emitter::Ptr SignalHandler::emitter() const
{
  W_PRV(const SignalHandler);
  return d->emitter;
}

int SignalHandler::signal() const
{
  W_PRV(const SignalHandler);
  return d->signum;
}
