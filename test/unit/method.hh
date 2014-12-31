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
#include "wintermutecore/method.hpp"
#include "wintermutecore/message.hpp"
#include "wintermutecore/util/serializable.hpp"

using Wintermute::Logging;
using Wintermute::Method;
using Wintermute::Message;
using Wintermute::Module;
using Wintermute::Util::Serializable;
using std::make_pair;

class MethodTestSuite : public CxxTest::TestSuite
{
public:
  void testBuildMethod()
  {
    Module::Ptr sampleModule(new SampleModule);
    Method::Ptr sampleMethod;
    Method::Arguments args;
    TS_ASSERT_THROWS_NOTHING ( sampleMethod = std::make_shared<Method>(
      "sample",
      sampleModule->designation(),
      sampleModule->designation(),
      args) );
  }

  void testInvokeMethod()
  {
    Module::Ptr sampleModule(new SampleModule);
    sampleModule->enable();

    Module::Call::Ptr sampleModuleCall(new SampleVoidModuleCall(sampleModule));
    sampleModule->addCall(sampleModuleCall);

    Method::Ptr sampleMethod;
    Method::Arguments args;

    TS_ASSERT_THROWS_NOTHING ( sampleMethod = std::make_shared<Method>(
      sampleModuleCall->name(),
      sampleModule->designation(),
      sampleModule->designation(),
      args) );
    TS_ASSERT_THROWS_NOTHING ( sampleMethod->invoke() );
  }

  void testCheckInvokabilityOfMethod()
  {
    Module::Ptr sampleModule(new SampleModule);
    sampleModule->enable();

    Module::Call::Ptr sampleModuleCall(new SampleVoidModuleCall(sampleModule));
    sampleModule->addCall(sampleModuleCall);

    Method::Ptr sampleMethod;
    Method::Arguments args;

    TS_ASSERT_THROWS_NOTHING ( sampleMethod = std::make_shared<Method>(
      sampleModuleCall->name(),
      sampleModule->designation(),
      sampleModule->designation(),
      args) );
    TS_ASSERT ( sampleMethod->isInvokable() );
    sampleModule->disable();
  }

  void testUseInternalArgumentsWhenNoneAreProvided()
  {
    Module::Ptr sampleModule(new SampleModule);
    sampleModule->enable();

    Module::Call::Ptr sampleModuleCall(new SampleMimicModuleCall(sampleModule));
    TS_ASSERT ( sampleModule->addCall(sampleModuleCall) );

    Method::Ptr sampleMethod;
    Method::Arguments args, args2;
    args.insert(make_pair("oatmeal raisin","cookies"));
    args2.insert(make_pair("fruit","cake"));

    TS_ASSERT_THROWS_NOTHING ( sampleMethod = std::make_shared<Method>(
      sampleModuleCall->name(),
      sampleModule->designation(),
      sampleModule->designation(),
      args) );
    TS_ASSERT_EQUALS ( sampleMethod->invoke(), Serializable::toString(args) );
    TS_ASSERT_EQUALS ( sampleMethod->invoke(args2), Serializable::toString(args2) );
    sampleModule->disable();
  }

  void testSerializedStructure()
  {
    Method::Ptr sampleMethod;
    Method::Arguments args;
    Serializable::Map serializedMethod;
    Module::Ptr sampleModule(new SampleModule);
    Module::Call::Ptr sampleModuleCall(new SampleMimicModuleCall(sampleModule));

    args.insert(make_pair("oatmeal raisin","cookies"));
    TS_ASSERT_THROWS_NOTHING ( sampleMethod = std::make_shared<Method>(
      sampleModuleCall->name(),
      sampleModule->designation(),
      sampleModule->designation(),
      args) );

    serializedMethod = static_cast<Serializable::Map>(*sampleMethod);
    Message::HashType moduleMap = Serializable::fromString(serializedMethod["module"]);

    TS_ASSERT_EQUALS ( serializedMethod["name"], sampleModuleCall->name() );
    TS_ASSERT_EQUALS ( serializedMethod["args"], Serializable::toString(args) );
    TS_ASSERT_EQUALS ( moduleMap["from"], static_cast<string>(sampleModule->designation()) );
    TS_ASSERT_EQUALS ( moduleMap["to"], static_cast<string>(sampleModule->designation()) );
  }

  void testCraftMessageForMethodCall()
  {
    Wintermute::Message msg;
    Method::Ptr sampleMethod;
    Method::Arguments args;

    Module::Ptr sampleModule(new SampleModule);
    Module::Call::Ptr sampleModuleCall(new SampleMimicModuleCall(sampleModule));

    args.insert(make_pair("oatmeal raisin","cookies"));
    TS_ASSERT_THROWS_NOTHING ( sampleMethod = std::make_shared<Method>(
      sampleModuleCall->name(),
      sampleModule->designation(),
      sampleModule->designation(),
      args) );

    TS_ASSERT_THROWS_NOTHING ( msg = sampleMethod->toMessage() );
    TS_ASSERT_EQUALS ( static_cast<string>(msg.sender()),
      static_cast<string>(sampleModule->designation()) );
    TS_ASSERT_EQUALS ( static_cast<string>(msg.receiver()),
      static_cast<string>(sampleModule->designation()) );

  }

};

