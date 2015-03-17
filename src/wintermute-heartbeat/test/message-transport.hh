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
#include "../globals.hpp"
#include "../messages.hpp"
#include <wintermutecore/events.hpp>
#include <wintermutecore/tunnel.hpp>

using Wintermute::Events::Timer;
using Wintermute::Events::Event;
using Wintermute::Events::Loop;
using Wintermute::Tunnel;
using Wintermute::Message;
using Wintermute::Heartbeat::PongMessage;
using Wintermute::Heartbeat::PingMessage;
using std::placeholders::_1;

class HeartbeatMessageDeliveryTestSuite : public CxxTest::TestSuite
{
public:
  bool passed = false;
  Plugin::Ptr pluginPtr;
  const string pongerName = WINTERMUTE_HEARTBEAT_MODULE_PONGER;
  const string pingerName = WINTERMUTE_HEARTBEAT_MODULE_PINGER;

  void setUp()
  {
    passed = false;

    Wintermute::Module::Pool::instance();
    Tunnel::instance();

    TS_ASSERT ( !pluginPtr );
    pluginPtr = wntrtest_load_plugin ( "wintermute-heartbeat" );
  }

  void tearDown()
  {
    passed = false;

    wntrtest_unload_plugin ( WINTERMUTE_HEARTBEAT_PLUGIN_NAME );
    pluginPtr = nullptr;
    TS_ASSERT ( pluginPtr == nullptr );
  }

  bool checkStructureOfPongMessage(const Message& msg)
  {
    const auto sender = msg.sender();
    const auto receiver = msg.receiver();

    TSM_ASSERT_EQUALS (
      "PongMessage was sent from ponger module.",
      sender.name(),
      pongerName
    );

    TSM_ASSERT_EQUALS (
      "PongMessage was sent to pinger module.",
      receiver.name(),
      pingerName
    );

    return true;
  }

  bool checkStructureOfPingMessage(const Message& msg)
  {
    const auto sender = msg.sender();
    const auto receiver = msg.receiver();

    TSM_ASSERT_EQUALS (
      "PingMessage was sent from pinger module.",
      sender.name(),
      pingerName
    );

    TSM_ASSERT_EQUALS (
      "PingMessage was sent to ponger module.",
      receiver.name(),
      pongerName
    );

    return true;
  }

  void checkForMessage(const Tunnel::MessageEvent::Direction &messageDirection,
    std::function<bool(const Wintermute::Message& message)> messageCheckFunc)
  {
    Tunnel::start();
    auto loop = Loop::primary();
    auto checkTunnelFunc = [this, &loop, &messageCheckFunc, &messageDirection](const Event::Ptr& event)
    {
      auto msgPtr = dynamic_pointer_cast<Tunnel::MessageEvent>(event);
      assert(msgPtr);

      if (msgPtr->direction == messageDirection)
      {
        this->passed = messageCheckFunc(msgPtr->message);

        if (this->passed)
        {
          loop->stop();
        }
      }
    };

    auto timerExpiredFunc = [&loop](const Event::Ptr& timerEvent)
    {
      assert(timerEvent);
      TS_TRACE("Timeout reached; breaking out...");
      loop->stop();
    };

    Timer::Ptr timerKill = make_shared<Timer>(loop);
    auto listener =
      Tunnel::instance()->listenForEvent(W_EVENT_TUNNEL_MESSAGE, checkTunnelFunc);
    timerKill->setInterval(KILL_TEST_TIMEOUT);
    timerKill->listenForEvent(W_EVENT_TIMEOUT, timerExpiredFunc);
    timerKill->start();

    loop->run();

    if (!passed)
    {
      TS_FAIL("No message was sent by any of the heartbeat modules.");
    }

    Tunnel::instance()->removeEventListener(listener);
    Tunnel::stop();
  }

  void testPongMessageTransport()
  {
    auto sampleDaemonModulePtr = make_shared<SampleModule>(Wintermute::Module::Designation("daemon","in.wintermute"));
    sampleDaemonModulePtr->enable();
    auto checkMsgFunc = std::bind(&HeartbeatMessageDeliveryTestSuite::checkStructureOfPongMessage, *this, _1);
    checkForMessage(Tunnel::MessageEvent::DirectionOutgoing, checkMsgFunc);
    sampleDaemonModulePtr->disable();
    sampleDaemonModulePtr = nullptr;
  }

  void testPingMessageTransport()
  {
    auto checkMsgFunc = std::bind(&HeartbeatMessageDeliveryTestSuite::checkStructureOfPingMessage, *this, _1);
    checkForMessage(Tunnel::MessageEvent::DirectionOutgoing, checkMsgFunc);
  }
};
