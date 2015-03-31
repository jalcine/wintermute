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
#include "wintermutecore/tunnel.hpp"

using std::string;
using Wintermute::Module;
using Wintermute::Message;
using Wintermute::Tunnel;
using Wintermute::Logging;
using namespace Wintermute::Events;

class TunnelTestSuite : public CxxTest::TestSuite
{
public:
  void setUp(void)
  {
  }

  void testRaisesEvents()
  {
    /// TODO: Check for 'W_EVENT_TUNNEL_START'.
    /// TODO: Check for 'W_EVENT_TUNNEL_STOP'.
    /// TODO: Check for 'W_EVENT_TUNNEL_MESSAGE'.
  }

  void testFindADispatcher(void)
  {
    Tunnel::Dispatcher::Ptr dispatcherPtr(new SampleDispatcher);
    TS_ASSERT(Tunnel::registerDispatcher(dispatcherPtr));
    TS_ASSERT(Tunnel::knowsOfDispatcher("sample"));
    TS_ASSERT(Tunnel::unregisterDispatcher(dispatcherPtr));
    TS_ASSERT(!Tunnel::unregisterDispatcher("foobarzilla"));
    TS_ASSERT(!Tunnel::knowsOfDispatcher("foobarzilla"));
  }

  void testAddAndFindAReceiver(void)
  {
    Wintermute::Message msg;
    Tunnel::Receiver::Ptr receiverPtr(new SampleReceiver(msg));
    TS_ASSERT(Tunnel::registerReceiver(receiverPtr));
    TS_ASSERT(Tunnel::knowsOfReceiver("sample"));
    TS_ASSERT(Tunnel::unregisterReceiver(receiverPtr));
    TS_ASSERT(!Tunnel::unregisterReceiver("foobarzilla"));
    TS_ASSERT(!Tunnel::knowsOfReceiver("foobarzilla"));
  }

  void testCleanOutReceivers(void)
  {
    Wintermute::Message msg;
    for (int i = 1; i < 10; ++i)
    {
      string newName = "sampleReceiver_" + std::to_string(i);
      Tunnel::Receiver::Ptr receiverPtr(new SampleReceiver(msg, newName));
      TS_ASSERT(Tunnel::registerReceiver(receiverPtr));
      TS_ASSERT_EQUALS(receiverPtr->emitter()->listeners(W_EVENT_TUNNEL_MESSAGE).size(), 1);
    }

    TS_ASSERT_EQUALS ( Tunnel::receivers().size(), 9 );
    Tunnel::clearAllReceivers();
    TS_ASSERT_EQUALS ( Tunnel::receivers().size(), 0 );
  }

  void testCleanOutDispatchers(void)
  {
    Tunnel::clearAllReceivers();
    Tunnel::clearAllDispatchers();
    TS_ASSERT_EQUALS ( Tunnel::dispatchers().size(), 0 );
    for (int i = 1; i < 10; ++i)
    {
      string newName = "sampleDispatcher_" + std::to_string(i);
      Tunnel::Dispatcher::Ptr dispatcherPtr(new SampleDispatcher(newName));
      TS_ASSERT(Tunnel::registerDispatcher(dispatcherPtr));
      TS_ASSERT_EQUALS(Tunnel::instance()->emitter()->listeners(W_EVENT_TUNNEL_MESSAGE).size(), i+1);
    }

    TS_ASSERT_EQUALS ( Tunnel::dispatchers().size(), 9 );
    Tunnel::clearAllDispatchers();
    TS_ASSERT_EQUALS ( Tunnel::dispatchers().size(), 0 );
  }
};


