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
#include "wintermutecore/event_loop.hh"

using Wintermute::Events::Loop;
using Wintermute::Events::LoopPrivate;

class SampleLoop : public Wintermute::Events::Loop
{
public:
  inline uv_loop_t* uvLoop()
  {
    return d_func()->loop;
  }
};

class EventLoopTestSuite : public CxxTest::TestSuite
{
public:
  void testCanStartNoopLoops()
  {
    Loop::Ptr loop = make_shared<Loop>();
    TS_ASSERT ( loop );
    TS_ASSERT ( !loop->isRunning() );
    TS_ASSERT ( loop->run() );
    TS_ASSERT ( !loop->isRunning() );
  }

  void testCanUseDefaultLoop()
  {
    Loop::Ptr loop        = make_shared<Loop>(true);
    Loop::Ptr anotherLoop = make_shared<Loop>(false);
    Loop::Ptr defaultLoop = Loop::primary();
    TS_ASSERT ( loop );
    TS_ASSERT ( anotherLoop );

    TSM_ASSERT_EQUALS ( "Both equate to 'uv_default_loop'.",
      std::static_pointer_cast<SampleLoop>(loop)->uvLoop(),
      uv_default_loop() );
    TS_ASSERT ( !anotherLoop->isPrimary() );
    TS_ASSERT ( loop->isPrimary() );
    TS_ASSERT ( defaultLoop->isPrimary() );
  }
};



