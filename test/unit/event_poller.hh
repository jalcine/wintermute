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

#include <cstdio>
#include <chrono>
#include <ctime>
#include "test_suite.hpp"
#include "wintermute-core/events.hpp"

using Wintermute::Events::Event;
using Wintermute::Events::Loop;
using Wintermute::Events::Poller;
using std::dynamic_pointer_cast;

SharedPtr<uv_pipe_t> open_local_socket_for_test(uv_loop_t* loopPtr)
{
  auto nowTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  string nowStr = ctime(&nowTime);
  string pathToSocket = TEST_BASE_DIR "/wintermute-test-" + nowStr + ".sock\0";
  SharedPtr<uv_pipe_t> pipePtr = make_shared<uv_pipe_t>();
  W_CHECK_UV(uv_pipe_init(loopPtr, pipePtr.get(), 0), "uv_pipe_init");
  W_CHECK_UV(uv_pipe_bind(pipePtr.get(), pathToSocket.c_str() ), "uv_pipe_bind");
  return pipePtr;
}

class EventPollerTestSuite : public CxxTest::TestSuite
{
public:
  void RequiresAValidEventLoop()
  {
    Poller::Ptr poller, poller2;
    SampleLoop::Ptr loop = make_shared<SampleLoop>();
    uv_loop_t* loopPtr = loop->uvLoop();
    int fileDesc = 0, r = 0;

    TS_ASSERT ( loop );

    SharedPtr<uv_pipe_t> pipePtr = open_local_socket_for_test(loopPtr);
    r = uv_fileno((uv_handle_t*)pipePtr.get(), &fileDesc);
    W_CHECK_UV(r, "uv_fileno");

    TS_ASSERT_DIFFERS ( fileDesc, 0 );

    TS_ASSERT_THROWS_NOTHING (
      poller = make_shared<Poller>(fileDesc, Poller::PollReadable, loop)
    );

    TS_ASSERT_THROWS (
      poller2 = make_shared<Poller>(0, Poller::PollReadable, nullptr),
      std::invalid_argument
    );

    uv_close((uv_handle_t*)pipePtr.get(), NULL);
  }

  void testCanListenToFds()
  {
    bool isCalled = false;
    int fileDesc = 0, r = 0;
    Loop::Ptr loop = Loop::primary();
    Poller::Ptr poller;
    auto invokeFunction = [&isCalled, &poller](const Event::Ptr & eventPtr) -> void
    {
      TS_TRACE(eventPtr->name());
      TS_TRACE(poller->stop());
      isCalled = true;
    };

    SharedPtr<uv_pipe_t> pipePtr = open_local_socket_for_test(uv_default_loop());
    r = uv_fileno((uv_handle_t*)pipePtr.get(), &fileDesc);
    W_CHECK_UV(r, "uv_fileno");
    TS_ASSERT_DIFFERS ( fileDesc, 0 );

    poller = make_shared<Poller>(fileDesc, Poller::PollReadable, loop);
    TS_ASSERT ( poller );
    poller->listenForEvent("core.events.poll", invokeFunction, Listener::FrequencyOnce);

    TSM_ASSERT ( "Poller started.", poller->start() );
    TSM_ASSERT ( "Loop ran.", loop->run() );
    TSM_ASSERT ( "Event 'core.events.poll' was emitted by poller.", isCalled );

    uv_close((uv_handle_t*)pipePtr.get(), NULL);
  }
};
