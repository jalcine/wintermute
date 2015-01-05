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

#include <algorithm>
#include "test_suite.hpp"
#include "wintermutecore/events.hpp"

using std::end;
using std::find;
using std::begin;
using Wintermute::Events::Event;
using Wintermute::Events::Loop;
using Wintermute::Events::Listener;
using Wintermute::Events::Emitter;

class EventEmitterTestSuite : public CxxTest::TestSuite
{
public:
  void testRequiresAValidEventLoop()
  {
    Emitter::Ptr emitter, emitter2;
    Loop::Ptr loop = make_shared<Loop>(true);
    TS_ASSERT_THROWS_NOTHING (
      emitter = make_shared<Emitter>(loop)
    );

    TS_ASSERT_THROWS (
      emitter2 = make_shared<Emitter>(nullptr),
      std::invalid_argument
    );
  }

  void testAddListeners()
  {
    Listener::Callback cb = [&](const Event::Ptr & event) -> void
    {
      assert(event);
    };
    Listener::Ptr aListener = make_shared<Listener>(cb),
      aListener2 = make_shared<Listener>(cb),
      aListener3 = make_shared<Listener>(cb);
    Loop::Ptr loop = make_shared<Loop>(true);
    Emitter::Ptr emitter = make_shared<Emitter>(loop);

    TS_ASSERT_THROWS_NOTHING ( emitter->listen("sample", aListener) );
    TS_ASSERT_THROWS_NOTHING ( emitter->listen("sample", aListener2) );
    TS_ASSERT_THROWS_NOTHING ( emitter->listen("sample", aListener3) );
    auto sampleListeners = emitter->listeners("sample");

    TS_ASSERT_EQUALS ( sampleListeners.size(), 3 );
    TS_ASSERT_DIFFERS (
      find(begin(sampleListeners), end(sampleListeners), aListener),
      end(sampleListeners)
    );
    TS_ASSERT_DIFFERS (
      find(begin(sampleListeners), end(sampleListeners), aListener2),
      end(sampleListeners)
    );
    TS_ASSERT_DIFFERS (
      find(begin(sampleListeners), end(sampleListeners), aListener3),
      end(sampleListeners)
    );
  }

  void testRemoveListeners()
  {
    Listener::Callback cb = [&](const Event::Ptr & event) -> void
    {
      assert(event);
    };
    Listener::Ptr aListener = make_shared<Listener>(cb);
    Loop::Ptr loop = make_shared<Loop>(true);
    Emitter::Ptr emitter = make_shared<Emitter>(loop);

    TS_ASSERT_THROWS_NOTHING ( emitter->listen("sample", aListener) );
    TS_ASSERT_THROWS_NOTHING ( emitter->stopListening(aListener) );
    auto sampleListeners = emitter->listeners("sample");
    TS_ASSERT_EQUALS (
      find(begin(sampleListeners), end(sampleListeners), aListener),
      end(sampleListeners)
    );
  }

  void testInvokeListeners()
  {
    Listener::Callback cb = [&](const Event::Ptr & event) -> void
    {
      assert(event);
    };
    Listener::Ptr onceListener = make_shared<Listener>(cb),
      allListener = make_shared<Listener>(cb);
    Loop::Ptr loop = make_shared<Loop>(true);
    Event::Ptr event = make_shared<SampleEvent>("sample");
    Emitter::Ptr emitter = make_shared<Emitter>(loop);

    onceListener->frequency = Listener::FrequencyOnce;
    allListener->frequency = Listener::FrequencyEvery;

    TS_ASSERT_THROWS_NOTHING ( emitter->listen("sample", onceListener) );
    TS_ASSERT_THROWS_NOTHING ( emitter->listen("sample", allListener) );
    TS_ASSERT_THROWS_NOTHING ( emitter->emit(event) );
    TS_ASSERT_THROWS_NOTHING ( emitter->emit(event) );

    auto sampleListeners = emitter->listeners("sample");
    auto allListenerItr = find(begin(sampleListeners), end(sampleListeners), allListener);
    auto onceListenerItr = find(begin(sampleListeners), end(sampleListeners), onceListener);

    TS_ASSERT_DIFFERS (
      allListenerItr,
      end(sampleListeners)
    );
    TS_ASSERT_EQUALS (
      onceListenerItr,
      end(sampleListeners)
    );
  }
};
