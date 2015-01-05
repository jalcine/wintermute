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

using Wintermute::Logging;
using Wintermute::Plugin;
using Wintermute::Tunnel;
using Wintermute::Plugin;
using std::dynamic_pointer_cast;

class TestZMQReceiver : public Wintermute::ZMQReceiver
{
  public:
    explicit TestZMQReceiver(SharedPtr<zmq::context_t>& context) :
      ZMQReceiver(context), message()
    {
    }

    virtual ~TestZMQReceiver()
    {
    }

    Wintermute::Message message;
    bool wasCalled = false;

    inline virtual Wintermute::Message receive()
    {
      message = Wintermute::ZMQReceiver::receive();
      wdebug(message);
      wasCalled = true;
      return message;
    }
};

class ZMQTunnelTestSuite : public CxxTest::TestSuite
{
public:
  Plugin::Ptr pluginPtr;
  void setUp()
  {
    setenv(WINTERMUTE_ENV_PLUGIN_PATH, string(TEST_BASE_DIR "/../lib").c_str(), 1);
    wdebug(TEST_BASE_DIR);
    pluginPtr = Plugin::find("transport-zmq");
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
    SharedPtr<Wintermute::ZMQPlugin> zmqPluginPtr =
      dynamic_pointer_cast<Wintermute::ZMQPlugin>(pluginPtr);
    SharedPtr<TestZMQReceiver> sampleTestZMQReceiver =
      make_shared<TestZMQReceiver>(zmqPluginPtr->context);
    Wintermute::Message msg = craftRandomMessage();

    TSM_ASSERT ( "Remove the actual ZeroMQ receiver.",
      Tunnel::unregisterReceiver(WINTERMUTE_ZMQ_TUNNEL_NAME) );
    TSM_ASSERT ( "Add in our fixture version of the reciever.",
      Tunnel::registerReceiver(sampleTestZMQReceiver) );
    TSM_ASSERT ( "Our test receiver stubs in place for ZeroMQ.",
      Tunnel::knowsOfReceiver(WINTERMUTE_ZMQ_TUNNEL_NAME) );

    TS_SKIP (R"(
This test is skipped because the Tunnel doesn't have a sense of active polling.
This means every message that's sent isn't queued automatically. Re-enable this
test when the event loop is built into Wintermute or we find a way to implement
a mock event loop that can used for Wintermute in test mode.
)");

    TSM_ASSERT ( "Sends a message over the wire using ZeroMQ.",
      Tunnel::sendMessage ( msg ) );
    TSM_ASSERT ( "A (poor) check to confirm that the override is called.",
      sampleTestZMQReceiver->wasCalled );
    TSM_ASSERT ( "Ensure that we received a message via ZeroMQ.",
      !sampleTestZMQReceiver->message.isEmpty() );
    TSM_ASSERT_EQUALS ( "Confirm that message sent = message received via ZeroMQ.",
      sampleTestZMQReceiver->message, msg );
  }

};
