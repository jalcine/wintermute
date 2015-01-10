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
#include <algorithm>
#include <cstdio>
#include "test_suite.hpp"
#include "wintermutecore/events.hpp"

using std::end;
using std::find;
using std::begin;
using Wintermute::Events::Event;
using Wintermute::Events::Loop;
using Wintermute::Events::Listener;
using Wintermute::Events::Poller;
using Wintermute::Events::Poller;

class EventPollerTestSuite : public CxxTest::TestSuite
{
public:
  void testRequiresAValidEventLoop()
  {
    Poller::Ptr poller, poller2;
    Loop::Ptr loop = Loop::primary();

    TS_ASSERT ( loop );

    TS_ASSERT_THROWS_NOTHING (
      poller = make_shared<Poller>(1, Poller::PollReadable, loop)
    );

    TS_ASSERT_THROWS (
      poller2 = make_shared<Poller>(0, Poller::PollReadable, nullptr),
      std::invalid_argument
    );
  }

  void testCanListenToFds()
  {
    bool isCalled = false;
    auto invokeFunction = [&isCalled](const Event::Ptr& eventPtr) -> void
    {
      TS_TRACE(eventPtr->name());
      TS_TRACE("yo");
      isCalled = true;
    };
    std::FILE* file = fopen(SAMPLE_TEST_FILE, "r");
    Loop::Ptr loop = Loop::primary();
    TS_ASSERT ( file );

    Poller::Ptr poller = make_shared<Poller>(fileno(file), Poller::PollReadable, loop);
    TS_ASSERT ( poller );
    poller->listenForEvent("core.events.poll", invokeFunction, Listener::FrequencyOnce);

    TS_ASSERT ( poller->start() );
    TS_ASSERT ( loop->run() );
    TSM_ASSERT ( "Event 'core.events.poll' was emitted by poller.", isCalled );
  }
};
