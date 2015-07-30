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
#include <chrono>
#include <wintermute-core/util/serializable.hpp>
#include <wintermute-core/message.hpp>
#include <cxxtest/TestSuite.h>

using Wintermute::Util::Serializable;
using Wintermute::Message;
using Wintermute::Module;
using std::make_pair;

class MessageTestSuite : public CxxTest::TestSuite
{
public:
  string getCurrentTimeString() const
  {
    const time_t curTime = time(nullptr);
    string strTime = asctime(std::gmtime(&curTime));
    strTime.pop_back();
    strTime += " UTC";
    return strTime;
  }

  void testClone(void)
  {
    Message::HashType data;
    data.insert(std::make_pair("foo", "bar"));
    Message message;
    message.setPayload(data);
    TS_ASSERT_EQUALS( message.payload().at("foo"), Message::HashValue("bar") );

    Message clonedMessage(message.clone());
    TS_ASSERT_EQUALS( clonedMessage.payload().at("foo"), Message::HashValue("bar") );
  }

  void testEmpty(void)
  {
    Message aMessage;
    TS_ASSERT_EQUALS( aMessage.isEmpty(), true );
  }

  void testSchemaCheck(void)
  {
    const string timestampString = getCurrentTimeString();
    Module::Designation sender("input", "in.wintermute.test");
    Module::Designation receiver("output", "in.wintermute.test");
    Message::HashType data;
    data.insert(std::make_pair("foo", std::to_string(100)));

    Serializable::Map messageMap;
    messageMap.insert(make_pair("sender", sender));
    messageMap.insert(make_pair("receiver", receiver));
    messageMap.insert(make_pair("payload", Serializable::toString(data)));
    messageMap.insert(make_pair("timestamp", timestampString));

    Message message;
    message = messageMap;

    TS_ASSERT_EQUALS ( message.sender() , sender );
    TS_ASSERT_EQUALS ( message.receiver() , receiver );
    TS_ASSERT_EQUALS ( message.payload() , data );
    TS_ASSERT_EQUALS ( message.timestamp(), timestampString );
  }

  void testEnsureThatTimestampIsUtc()
  {
    Message message = craftRandomMessage();
    const string obtainedTimestring = message.timestamp();

    TS_ASSERT ( obtainedTimestring.find("UTC") != 0 );
    TS_ASSERT_EQUALS ( obtainedTimestring.find("UTC"),
        obtainedTimestring.length() - 3 );
  }
};
