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

#include "logging.hpp"
#include "event_loop.hh"
#include "event_signal_handler.hh"

using Wintermute::Events::Emitter;
using Wintermute::Events::SignalHandler;
using Wintermute::Events::SignalHandlerPrivate;

SignalHandlerPrivate::SignalHandlerPrivate( const int& signalNumber) :
  q_ptr(nullptr), signum(signalNumber), emitter(new Emitter)
{
  handle = make_shared<SignalType>();
  handle->data = (void*) this; // sigh.
}

SignalHandlerPrivate::~SignalHandlerPrivate()
{
  const int signalStopped = uv_signal_stop(handle.get());
  if (signalStopped != 0)
  {
    // TODO: Handle error.
  }
}

void SignalHandlerPrivate::attachSignal()
{
  Loop::Ptr loop = Loop::primary();
  const int signalCreated = uv_signal_init(loop->d_func()->loop, handle.get());
  if (signalCreated != 0)
  {
    wwarn("Failed to create signal handler.");
    // TODO: Handle error.
    return;
  }

  const int signalStarted = uv_signal_start(handle.get(), &w_event_signal_cb, signum);
  if (signalStarted != 0)
  {
    // TODO: Handle error.
    wwarn("Failed to start signal handler.");
    handle = nullptr;
    return;
  }
}
