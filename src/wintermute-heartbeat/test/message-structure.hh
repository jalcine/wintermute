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

#include <uuid/uuid.h>
#include <vector>
#include "test_suite.hpp"
#include "../messages.hpp"
#include "../module.hpp"
#include "../globals.hpp"
#include <wintermutecore/util.hh>
#include <wintermutecore/module.hpp>

using std::vector;
using Wintermute::Module;
using Wintermute::Util::split_string;
using Message = Wintermute::Message;
using MessageMap = Wintermute::Util::Serializable::Map;
using HPongMessage = Wintermute::Heartbeat::PongMessage;
using HPingMessage = Wintermute::Heartbeat::PingMessage;

class HeartbeatMessageStructureTestSuite : public CxxTest::TestSuite
{
public:
  Plugin::Ptr pluginPtr = nullptr;
  Module::Ptr modulePtr = nullptr;

  void setUp()
  {
    TS_ASSERT ( !pluginPtr );
    pluginPtr = wntrtest_load_plugin ( "wintermute-heartbeat" );
    modulePtr = make_shared<Wintermute::Heartbeat::Module>();
    TS_ASSERT(modulePtr);
    TS_ASSERT(modulePtr->enable());
    TS_ASSERT(modulePtr->isEnabled());
  }

  void tearDown()
  {
    modulePtr->disable();
    modulePtr = nullptr;
    pluginPtr = nullptr;
    wntrtest_unload_plugin ( WINTERMUTE_HEARTBEAT_PLUGIN_NAME );
    TS_ASSERT ( pluginPtr == nullptr );
  }

  MessageMap fetchPongMessage()
  {
    Message msg = HPongMessage::craft();
    return (MessageMap) (msg.payload());
  }

  MessageMap fetchPingMessage()
  {
    Message msg = HPingMessage::craft();
    return (MessageMap) (msg.payload());
  }

  void checkKeysInMap(const vector<string>& keys, const MessageMap& payload)
  {
    for ( const string expectedKey : keys )
    {
      TSM_ASSERT (
        "The key '" + expectedKey + "' exists in the message payload.",
        payload.count(expectedKey) == 1
      );
    }
  }

  void cycleHeartbeatPlugin()
  {
    tearDown();
    setUp();
  }

  void testCheckForProperKeysInPongMessage()
  {
    MessageMap msg = fetchPongMessage();
    const vector<string> expectedKeys = { "uuid" };

    checkKeysInMap(expectedKeys, msg);
  }

  void testCheckForProperKeysInPingMessage()
  {
    MessageMap msg = fetchPingMessage();
    const vector<string> expectedKeys = { "plugins", "modules" };

    checkKeysInMap(expectedKeys, msg);
  }

  void testCheckPingMessagePluginValue()
  {
    MessageMap msg = fetchPingMessage();
    const vector<string> expectedPlugins = { "wintermute-heartbeat" };
    const list<string> obtainedPlugins =
      split_string(msg.find("plugins")->second, std::regex(","));

    TSM_ASSERT (
      "The 'plugins' key has the currently loaded plugins' names.",
      std::equal (
        expectedPlugins.cbegin(), expectedPlugins.cend(),
        obtainedPlugins.begin()
      )
    );
  }

  void testCheckPingMessageModulesValue()
  {
    Wintermute::Module::Ptr sampleModule = make_shared<SampleModule>();
    sampleModule->enable();
    MessageMap msg = fetchPingMessage();

    const vector<string> expectedModules = {
      sampleModule->designation().domain() + "." + sampleModule->designation().name()
    };
    const list<string> obtainedModules =
      split_string(msg.find("modules")->second, std::regex(","));

    TSM_ASSERT (
      "The 'modules' key has the currently loaded modules' names.",
      std::equal (
        expectedModules.cbegin(), expectedModules.cend(),
        obtainedModules.begin()
      )
    );
  }

  void testCheckPongMessageUuid()
  {
    MessageMap msg = fetchPongMessage();
    const string obtainedUuid = msg.find("uuid")->second;

    TSM_ASSERT (
      "The UUID value is not empty.",
      obtainedUuid.empty() == false
    );

    TSM_ASSERT (
      "The UUID value is as expected.",
      true /*TODO Check if pong message returns a UUID.*/
    );
  }

  void testCheckPongMessageUuidChanges()
  {
    MessageMap msg = fetchPongMessage();
    const string obtainedUuid = msg.find("uuid")->second;

    TSM_ASSERT (
      "The UUID value is not empty.",
      obtainedUuid.empty() == false
    );

    TSM_ASSERT_THROWS_NOTHING (
      "Cycle the Heartbeat plugin.",
      cycleHeartbeatPlugin()
    );

    MessageMap expectedMsg = fetchPongMessage();
    const string expectedUuid = expectedMsg.find("uuid")->second;

    TSM_ASSERT_DIFFERS (
      "The Heartbeat plugin generates different UUIDs.",
      obtainedUuid,
      expectedUuid
    );
  }
};
