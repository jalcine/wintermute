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
#include "../plugin.hpp"
#include "../receiver.hpp"
#include <wintermutecore/tunnel.hpp>
#include <wintermutecore/plugin.hpp>
#include <wintermutecore/logging.hpp>

using Wintermute::Logging;
using Wintermute::Plugin;
using Wintermute::Tunnel;
using Wintermute::Plugin;
using std::dynamic_pointer_cast;

#define KILL_TEST_TIMEOUT 200
#define KILL_COUNT        40

class TestZMQReceiver : public Wintermute::ZMQReceiver
{
  public:
    explicit TestZMQReceiver(const SharedPtr<zmqpp::context>& aContext) :
      ZMQReceiver(aContext),
      message(Wintermute::Message())
    {
      wasCalled = true;
      wdebug("Spun up a sample ZeroMQ receiver.");
    }

    virtual ~TestZMQReceiver()
    {
      wdebug("Spun down a sample ZeroMQ receiver.");
    }

    Wintermute::Message message;
    bool wasCalled = false;
};

class ZMQTunnelTestSuite : public CxxTest::TestSuite
{
public:
  Plugin::Ptr pluginPtr;
  void setUp()
  {
    TS_ASSERT ( !Plugin::hasPlugin(WINTERMUTE_ZMQ_PLUGIN_NAME) );
    const string libDir(TEST_BASE_DIR "/../lib");
    setenv(WINTERMUTE_ENV_PLUGIN_PATH, libDir.c_str() , 1);
    pluginPtr = Plugin::find("transport-zmq");
    TS_ASSERT ( pluginPtr );
    werror ( "What? " + to_string(pluginPtr.unique()));
    TS_ASSERT ( Plugin::hasPlugin(WINTERMUTE_ZMQ_PLUGIN_NAME) );
    TS_ASSERT ( Tunnel::knowsOfDispatcher(WINTERMUTE_ZMQ_TUNNEL_NAME) );
    TS_ASSERT ( Tunnel::knowsOfReceiver(WINTERMUTE_ZMQ_TUNNEL_NAME) );
    unsetenv(WINTERMUTE_ENV_PLUGIN_PATH);
  }

  void tearDown()
  {
    TSM_ASSERT ( "Freed plugin",
      Plugin::release(WINTERMUTE_ZMQ_PLUGIN_NAME) );
    TSM_ASSERT ( "Removed plugin.",
      !Plugin::hasPlugin(WINTERMUTE_ZMQ_PLUGIN_NAME) );

    TSM_ASSERT ( "Removed dispatcher.",
      !Tunnel::knowsOfDispatcher(WINTERMUTE_ZMQ_TUNNEL_NAME) );
    TSM_ASSERT ( "Removed receiver.",
      !Tunnel::knowsOfReceiver(WINTERMUTE_ZMQ_TUNNEL_NAME) );
  }

  void testSendOutMessage()
  {
    bool passed = false;
    Loop::Ptr loop = Loop::primary();
    Wintermute::Message msg = craftRandomMessage();

    SharedPtr<Wintermute::ZMQPlugin> zmqPluginPtr =
      dynamic_pointer_cast<Wintermute::ZMQPlugin>(pluginPtr);

    SharedPtr<TestZMQReceiver> sampleTestZMQReceiver =
      make_shared<TestZMQReceiver>(zmqPluginPtr->context);

    TSM_ASSERT (
      "Sample receiver built properly.",
      sampleTestZMQReceiver
    );

    SharedPtr<Wintermute::ZMQDispatcher> theZMQDispatcher =
      dynamic_pointer_cast<Wintermute::ZMQDispatcher>(
        zmqPluginPtr->dispatcher->shared_from_this()
      );

    TSM_ASSERT (
      "Remove the default receiver.",
      Tunnel::unregisterReceiver(WINTERMUTE_ZMQ_TUNNEL_NAME)
    );

    TSM_ASSERT (
      "No receiver exists from ZeroMQ.",
      !Tunnel::knowsOfReceiver(WINTERMUTE_ZMQ_TUNNEL_NAME)
    );

    TSM_ASSERT (
      "Add in our fixture version of the receiver.",
      Tunnel::registerReceiver(sampleTestZMQReceiver)
    );

    TSM_ASSERT (
      "Our test receiver stubs in place for ZeroMQ.",
      Tunnel::knowsOfReceiver(WINTERMUTE_ZMQ_TUNNEL_NAME)
    );

    TSM_ASSERT (
      "There's a dispatcher in place for ZeroMQ.",
      Tunnel::knowsOfDispatcher(WINTERMUTE_ZMQ_TUNNEL_NAME)
    );

    TSM_ASSERT (
      "The dispatcher's in good shape.",
      theZMQDispatcher
    );

    Tunnel::instance()->listenForEvent(W_EVENT_TUNNEL_MESSAGE,
    [&passed, &msg, &loop](const Wintermute::Events::Event::Ptr& event)
    {
      Tunnel::MessageEvent::Ptr msgPtr =
        std::dynamic_pointer_cast<Tunnel::MessageEvent>(event);

      assert(msgPtr);

      if (msgPtr->direction == Tunnel::MessageEvent::DirectionIncoming)
      {
        wdebug("[test] Obtained a MessageEvent.");
        wdebug("[test] Got " + static_cast<string>(msgPtr->message));
        TSM_ASSERT_EQUALS (
          "Confirm that message sent = message received via ZeroMQ.",
          msgPtr->message,
          msg
        );
        passed = true;
        loop->stop();
      }
    });

    TSM_ASSERT_RELATION(
      "Ensures that the Tunnel has a listener for incoming messages.",
      std::greater<size_t>,
      Tunnel::instance()->eventListeners(W_EVENT_TUNNEL_MESSAGE).size(),
      1
    );

    Timer::Ptr timerKill = make_shared<Timer>(Loop::primary());
    Timer::Ptr timerSend = make_shared<Timer>(Loop::primary());

    timerKill->listenForEvent(W_EVENT_TIMEOUT,
    [&loop](const Event::Ptr& timerEvent)
    {
      assert(timerEvent);
      TS_TRACE("Timeout reached; breaking out...");
      loop->stop();
    });

    timerSend->listenForEvent(W_EVENT_TIMEOUT,
    [&msg, &passed, &loop](const Event::Ptr& timerEvent)
    {
      assert(timerEvent);
      TSM_ASSERT_THROWS_NOTHING (
        "Sends a message over the wire using ZeroMQ.",
        Tunnel::sendMessage ( msg )
        );

      if (passed)
      {
        loop->stop();
      }
    });

    timerSend->setInterval(KILL_TEST_TIMEOUT / KILL_COUNT);
    timerSend->start();
    timerKill->start(KILL_TEST_TIMEOUT);
    Tunnel::start();

    loop->run();
    Tunnel::stop();

    if (!passed)
    {
      TS_FAIL("ZeroMQ receiver did not catch incoming message.");
    }
  }

};
