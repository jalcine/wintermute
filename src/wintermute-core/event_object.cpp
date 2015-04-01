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

#include "event_object.hh"
#include "events.hpp"

using namespace Wintermute::Events;

Event::Event(const string& eventName) : d_ptr(new EventPrivate)
{
  W_PRV(Event);
  d->name = eventName;
}

Event::~Event()
{
}

string Event::name() const
{
  W_PRV(const Event);
  return d->name;
}
