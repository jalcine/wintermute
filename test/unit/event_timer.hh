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

#include "test_suite.hpp"
#include "wintermutecore/events.hpp"

using Wintermute::Events::Event;
using Wintermute::Events::Loop;
using Wintermute::Events::Timer;

class EventTimerTestSuite : public CxxTest::TestSuite
{
public:
  void testRequiresAValidEventLoop()
  {
    Timer::Ptr timer, timer2;
    Loop::Ptr loop = Loop::primary();

    TS_ASSERT ( loop );

    TS_ASSERT_THROWS_NOTHING (
      timer = make_shared<Timer>(loop);
    );

    TS_ASSERT_THROWS (
      timer2 = make_shared<Timer>(nullptr),
      std::invalid_argument
    );

    TS_ASSERT ( loop->run() );
  }

  void testWorkWithIntervals()
  {
    Timer::Ptr timer;
    Loop::Ptr loop = Loop::primary();

    TS_ASSERT ( loop );
    TS_ASSERT_THROWS_NOTHING ( timer = make_shared<Timer>(loop) );

    timer->listenForEvent("core.events.timeout",
      [&](const Event::Ptr& event) -> void
    {
      TS_TRACE("We get here.");
      timer->stop();
    }, Listener::FrequencyEvery);
    timer->setInterval(0);
    timer->start(1);

    TS_ASSERT ( loop->run() );
  }
};
