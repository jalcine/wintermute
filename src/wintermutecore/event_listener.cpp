/*
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with Wintermute; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "event_listener.hh"
#include "events.hpp"
#include "logging.hpp"

using namespace Wintermute::Events;

Listener::Listener(Callback callback) throw (std::invalid_argument) :
  d_ptr(make_shared<ListenerPrivate>())
{
  d_ptr = make_shared<ListenerPrivate>();
  W_PRV(Listener);

  if (!callback)
  {
    throw std::invalid_argument("Invalid callback functions provided.");
  }

  d->callback = callback;
}

Listener::~Listener()
{
}

void Listener::invoke(const Event::Ptr& event) throw (std::invalid_argument)
{
  if (!event)
  {
    throw std::invalid_argument("An invalid Event pointer was provided.");
  }

  W_PRV(Listener);

  if (d->callback)
  {
    wdebug("Invoking a callback for " + event->name() + "...");
    d->callback(event);
  }
  else
  {
    wdebug("No callback set up for this listener. A potential bug?");
  }

  if (d->callback)
  {
    wdebug("Invoking callback for " + event->name() + "...");
    d->callback(event);
  }
}
