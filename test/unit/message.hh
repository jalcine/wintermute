#include "libwintermutecore/message.hpp"
#include <cxxtest/TestSuite.h>

using Wintermute::Message;
using Wintermute::Module;

class MessageTestSuite : public CxxTest::TestSuite
{
public:
  void testClone(void)
  {
    Message::HashType data;
    data.insert(std::make_pair("foo", "bar"));
    Message message;
    message.setPayload(data);
    TS_ASSERT( message.payload().at("foo") == Message::HashValue("bar") );

    Message clonedMessage = message.clone();
    TS_ASSERT( clonedMessage.payload().at("foo") == Message::HashValue("bar") );
  }

  void testEmpty(void)
  {
    Message aMessage;
    TS_ASSERT( aMessage.isEmpty() == true );
  }

  void testLocalByDefault(void)
  {
    Message aMessage;
    TS_ASSERT( aMessage.isLocal() == true );
  }

  void testABICheck(void)
  {
    Module::Designation sender("input");
    Module::Designation receiver("output");
    Message::HashType data;
    data.insert(std::make_pair("foo", "bar"));

    Message message(data, receiver, sender);

    TS_ASSERT ( message.sender() == sender );
    TS_ASSERT ( message.receiver() == receiver );
    TS_ASSERT ( message.payload() == data );
  }
};
