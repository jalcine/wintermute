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
#include <wintermutecore/tunnel.hpp>

using Wintermute::Tunnel;
using Wintermute::Message;

class TunnelReceiverTestSuite : public CxxTest::TestSuite
{
public:
  void testBuild(void)
  {
    Wintermute::Message msg;
    Tunnel::Receiver::Ptr receiver(new SampleReceiver(msg));
    TS_ASSERT ( receiver );
  }

  void testReceiveMessage(void)
  {
    Message msg = craftRandomMessage();
    bool pass = false;
    Tunnel::instance()->listenForEvent(W_EVENT_TUNNEL_MESSAGE,
      [&msg,&pass](const Event::Ptr& eventPtr)
    {
      Tunnel::MessageEvent::Ptr msgPtr =
        std::dynamic_pointer_cast<Tunnel::MessageEvent>(eventPtr);

      if (!msgPtr) return;

      if (msgPtr->direction == Tunnel::MessageEvent::DirectionIncoming)
      {
        TS_ASSERT_EQUALS ( msgPtr->message, msg );
        TS_TRACE ( msgPtr->message );
        pass = true;
        Tunnel::instance()->emitter()->loop()->stop();
      } else if (msgPtr->direction == Tunnel::MessageEvent::DirectionOutgoing)
      {
        // Faux dispatcher.
        msgPtr->direction = Tunnel::MessageEvent::DirectionIncoming;
        Tunnel::instance()->emitEvent(msgPtr);
      }
    });

    Tunnel::Receiver::Ptr receiverPtr(new SampleReceiver(msg));
    TS_ASSERT ( Tunnel::registerReceiver(receiverPtr) );
    TS_ASSERT_EQUALS (
      receiverPtr->eventListeners(W_EVENT_TUNNEL_MESSAGE).size(),
      1
    );
    TS_ASSERT_THROWS_NOTHING ( Tunnel::sendMessage(msg) );
    Tunnel::instance()->emitter()->loop()->run();
    TS_ASSERT ( Tunnel::unregisterReceiver(receiverPtr) );

    if (!pass) {
      TS_FAIL("The callback for fetching a incoming message wasn't called.");
    }
  }

};
