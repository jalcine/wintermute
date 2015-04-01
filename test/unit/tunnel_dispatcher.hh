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
 * ree Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "test_suite.hpp"
#include <wintermute-core/tunnel.hpp>

using Wintermute::Tunnel;
using Wintermute::Message;

class TunnelDispatcherTestSuite : public CxxTest::TestSuite
{
public:
  void testBuild()
  {
    Tunnel::Dispatcher::Ptr dispatcher(new SampleDispatcher);
    TS_ASSERT ( dispatcher );
  }

  void testSendOutMessage(void)
  {
    Tunnel::Dispatcher::Ptr dispatcherPtr(new SampleDispatcher);
    TS_ASSERT ( Tunnel::registerDispatcher(dispatcherPtr) );

    Message message = craftRandomMessage();
    TS_ASSERT_THROWS_NOTHING ( Tunnel::sendMessage(message) );

    dispatcherPtr->listenForEvent(W_EVENT_TUNNEL_MESSAGE,
      [&message](const Event::Ptr& eventPtr)
    {
      const Tunnel::MessageEvent::Ptr msgPtr =
        std::dynamic_pointer_cast<Tunnel::MessageEvent>(eventPtr);

      const Message obtainedMessage = msgPtr->message;
      TS_ASSERT ( !obtainedMessage.isEmpty() );
      TS_ASSERT_EQUALS ( message, obtainedMessage );
      Tunnel::instance()->emitter()->loop()->stop();
    });

    Tunnel::instance()->emitter()->loop()->run();
  }

};

