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
		TS_ASSERT_EQUALS( message.payload().at("foo"), Message::HashValue("bar") );

		Message clonedMessage = message.clone();
		TS_ASSERT_EQUALS( clonedMessage.payload().at("foo"), Message::HashValue("bar") );
	}

	void testEmpty(void)
	{
		Message aMessage;
		TS_ASSERT_EQUALS( aMessage.isEmpty(), true );
	}

	void testIsntLocalByDefault(void)
	{
		Message aMessage;
		TS_ASSERT ( aMessage.isLocal() == false );
	}

	void testSchemaCheck(void)
	{
		Module::Designation sender("input", "in.wintermute.test");
		Module::Designation receiver("output", "in.wintermute.test");
		Message::HashType data;
		data.insert(std::make_pair("foo", std::to_string(100)));

		Message message(data, receiver, sender);

		TS_ASSERT_EQUALS ( message.sender() , sender );
		TS_ASSERT_EQUALS ( message.receiver() , receiver );
		TS_ASSERT_EQUALS ( message.payload() , data );
		TS_ASSERT_EQUALS ( message.isLocal() , true );
	}
};
