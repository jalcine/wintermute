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

#include <algorithm>
#include "event_emitter.hh"
#include "events.hpp"
#include "logging.hpp"

using namespace Wintermute::Events;
using std::for_each;

Emitter::Emitter(const Loop::Ptr& providedLoop) :
  d_ptr(new EmitterPrivate)
{
  W_PRV(Emitter);

  if (providedLoop)
  {
    d->loop = providedLoop;
  }
  else
  {
    throw std::invalid_argument("Emitter requires a valid pointer to a Loop.");
  }
}

Emitter::~Emitter()
{
  W_PRV(Emitter);
  d->listeners.clear();
}

Loop::Ptr Emitter::loop() const
{
  W_PRV(const Emitter);
  return d->loop;
}

Listener::Ptr Emitter::listen(const string& eventName, Listener::Ptr& listener)
{
  W_PRV(Emitter);
  d->listeners.emplace(eventName, listener);
  return listener;
}

Listener::Ptr Emitter::listen(const string& eventName, Listener::Callback cb,
  const Listener::Frequency& freq)
{
  Listener::Ptr listener = make_shared<Listener>(cb);
  listener->frequency = freq;

  return listen(eventName, listener);
}

Listener::List Emitter::listeners(const string& eventName) const
{
  W_PRV(const Emitter);
  Listener::List listenersFound;
  auto rangeOfElements = d->listeners.equal_range(eventName);

  for_each(rangeOfElements.first, rangeOfElements.second,
    [&listenersFound](const Listener::Map::value_type& listenerItr)
  {
    auto listenerFound = listenerItr.second;
    listenersFound.push_back(listenerFound);
  });

  return listenersFound;
}

bool Emitter::stopListening(const Listener::Ptr& listener)
{
  W_PRV(Emitter);
  auto listenerItr = find_if(begin(d->listeners), end(d->listeners),
    [&listener](const Listener::Map::value_type& listenerPair)
  {
    const auto obtainedListener = listenerPair.second;
    return (obtainedListener == listener);
  });

  if (listenerItr != std::end(d->listeners))
  {
    wdebug("Removed found listener.");
    d->listeners.erase(listenerItr);
    return true;
  }

  return false;
}

void Emitter::emit(const Event::Ptr& event)
{
  wdebug("Invoking " + event->name() + "...");
  Listener::List listenersForEvent = listeners(event->name());

  if (!listenersForEvent.empty())
  {
    wdebug("Invoking " + event->name() + " with " +
      to_string(listenersForEvent.size()) + " listeners ...");
    assert(listener);
    listener->invoke(event);

    auto invokeListenerFunc = [&](Listener::Ptr & listener)
    {
      assert(listener);
      listener->invoke(event);

      if (listener->frequency == Listener::FrequencyOnce)
      {
        stopListening(listener);
      }
    };

    for_each(begin(listenersForEvent), end(listenersForEvent),
      invokeListenerFunc);
  }
}
