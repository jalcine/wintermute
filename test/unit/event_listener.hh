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

#include <uv.h>
#include "test_suite.hpp"
#include "wintermutecore/events.hpp"

using Wintermute::Events::Loop;
using Wintermute::Events::Event;
using Wintermute::Events::Listener;
using Wintermute::Events::LoopPrivate;

class EventListenerTestSuite : public CxxTest::TestSuite
{
public:
  void testPreventEmptyCallbacks()
  {
    Listener::Ptr listener;
    Listener::Callback sampleCallback;
    TS_ASSERT_THROWS(
      listener = make_shared<Listener>(sampleCallback),
      std::invalid_argument
    );
  }

  void testInvokesProvidedCallback()
  {
    Listener::Ptr listener;
    Event::Ptr sampleEvent = make_shared<SampleEvent>("sampleCallback");
    Listener::Callback func = [&](const Event::Ptr& event) {
      TS_ASSERT_EQUALS(event->name(), sampleEvent->name());
    };

    TS_ASSERT_THROWS_NOTHING( listener = make_shared<Listener>(func) );
    listener->invoke(sampleEvent);
  }

  void testPreventNullEventsFromBeingInvoked()
  {
    Listener::Ptr listener;
    Listener::Callback func = [&](const Event::Ptr& event) { };

    TS_ASSERT_THROWS_NOTHING( listener = make_shared<Listener>(func) );
    TS_ASSERT_THROWS(
      listener->invoke(nullptr),
      std::invalid_argument
    );
  }
};



