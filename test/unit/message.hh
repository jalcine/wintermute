#include "libwintermutecore/message.hpp"
#include <cxxtest/TestSuite.h>

using Wintermute::Procedure::Message;

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
  }
};
