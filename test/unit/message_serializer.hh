#include "libwintermutecore/message_serializer.hpp"
#include <cxxtest/TestSuite.h>

using Wintermute::Message;
using Wintermute::MessageSerializer;

class MessageSerializerTestSuite : public CxxTest::TestSuite
{
public:
  void testConvertMessageIntoString(void)
  {
    Message::HashType thePayload;
    thePayload.insert(std::make_pair("foo", "bar"));
    Message aMessage;
    aMessage.setPayload(thePayload);

    const string theJsonString = MessageSerializer::serialize(aMessage);
    TS_ASSERT(theJsonString.find("foo") != 0);
  }

  void testConvertStringToMessage(void)
  {
    const string theJsonString = "{\"sender\":{\"name\":\"output\",\"domain\":\"test01.wintermute.in\",\"pid\":300}, \"receiver\":{\"name\":\"input\",\"domain\":\"test01.wintermute.in\",\"pid\":300}, \"payload\":{\"foo\":\"bar\"}}";
    Message generatedMessage = MessageSerializer::deserialize(theJsonString);
    TS_ASSERT(generatedMessage.payload().empty() == false);
    TS_ASSERT(generatedMessage.payload().at("foo") == "bar");
    TS_ASSERT(generatedMessage.sender().name() == "output");
    TS_ASSERT(generatedMessage.receiver().name() == "input");
  }
};
