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
#include "../messages.hpp"
#include "../pong_module.hpp"
#include "../ping_module.hpp"
#include "../globals.hpp"
#include <wintermutecore/util.hh>
#include <wintermutecore/module.hpp>

using std::vector;
using std::pair;
using Wintermute::Module;
using Wintermute::Util::split_string;
using Wintermute::Message;
using MessageMap = Wintermute::Util::Serializable::Map;
using Wintermute::Heartbeat::PongMessage;
using Wintermute::Heartbeat::PingMessage;
using Wintermute::Heartbeat::PongModule;
using Wintermute::Heartbeat::PingModule;

class HeartbeatMessageStructureTestSuite : public CxxTest::TestSuite
{
public:
  Plugin::Ptr pluginPtr = nullptr;

  void setUp()
  {
    TS_ASSERT ( !pluginPtr );
    pluginPtr = wntrtest_load_plugin ( "wintermute-heartbeat" );
  }

  void tearDown()
  {
    pluginPtr = nullptr;
    wntrtest_unload_plugin ( WINTERMUTE_HEARTBEAT_PLUGIN_NAME );
    TS_ASSERT ( pluginPtr == nullptr );
  }

  void cycleHeartbeatPlugin()
  {
    tearDown();
    setUp();
  }

  MessageMap fetchPongMessage()
  {
    Module::Ptr modulePtr = make_shared<PongModule>();

    TSM_ASSERT_EQUALS (
      "There's no other PongModule loaded.",
      Module::Pool::instance()->find(modulePtr->designation()),
      nullptr
    );

    modulePtr->enable();
    Message msg = PongMessage::craft();
    modulePtr->disable();
    modulePtr.reset();
    return (MessageMap) (msg.payload());
  }

  pair<string, MessageMap> fetchPongMessageAndUuid()
  {
    auto modulePtr = make_shared<PongModule>();

    TSM_ASSERT_EQUALS (
      "There's no other PongModule loaded.",
      Module::Pool::instance()->find(modulePtr->designation()),
      nullptr
    );

    const string uuid = modulePtr->uuid;
    modulePtr->enable();
    Message msg = PongMessage::craft();
    modulePtr->disable();
    modulePtr.reset();
    return make_pair(uuid, (MessageMap) msg.payload());
  }

  MessageMap fetchPingMessage()
  {
    Message msg = PingMessage::craft();
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

  void testNoop() { }

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
    auto sampleModule = make_shared<SampleModule>();
    sampleModule->enable();

    list<string> expectedModules, obtainedModules;

    const auto moduleList = Module::Pool::instance()->modules();
    for_each(begin(moduleList), end(moduleList), [&](const Module::Ptr& module)
    {
      auto des = module->designation();
      string craftedName = des.domain() + "." + des.name();
      expectedModules.push_back(craftedName);
    });

    MessageMap msg = fetchPingMessage();
    const string moduleStr = msg.find("modules")->second;
    wdebug(moduleStr);
    if (!moduleStr.empty())
    {
      obtainedModules = split_string(moduleStr, std::regex(","));
    }

    TSM_ASSERT_EQUALS (
      "The 'modules' key has the currently loaded module names.",
      expectedModules,
      obtainedModules
    );
  }

  void testCheckPongMessageUuid()
  {
    auto uuidAndMsg = fetchPongMessageAndUuid();
    auto msg = uuidAndMsg.second;

    const string obtainedUuid = msg.find("uuid")->second;
    const string expectedUuid = uuidAndMsg.first;

    TSM_ASSERT_EQUALS (
      "The UUID value is as expected.",
      obtainedUuid,
      expectedUuid
    );

    TSM_ASSERT (
      "The UUID value is not empty.",
      !obtainedUuid.empty()
    );
  }

  void testCheckPongMessageUuidChanges()
  {
    /**
     * @NOTE: This test is a bit funny since we make a new `PongModule` each time.
     */
    MessageMap msg = fetchPongMessage();
    const string obtainedUuid = msg.find("uuid")->second;

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

    TSM_ASSERT (
      "The UUID value is not empty.",
      !obtainedUuid.empty()
    );
  }
};
