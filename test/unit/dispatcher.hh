#include "libwintermutecore/dispatcher.hpp"
#include "libwintermutecore/message.hpp"
#include <cxxtest/TestSuite.h>
#include "include/fixtures.hpp"

using Wintermute::Message;
using Wintermute::Dispatcher;

Message aMessage;

class SampleDispatcher : public Dispatcher
{
public:
	explicit SampleDispatcher() : Dispatcher()
	{
	}

	virtual string name() const
	{
		return "sample";
	}

	virtual bool send(const Message& message)
	{
		aMessage = message;
		TS_TRACE("We should send something.");
		return true;
	}
};


class DispatcherTestSuite : public CxxTest::TestSuite
{
public:
	void testABICheck(void)
	{
		SampleDispatcher dispatcher;
		dispatcher.send(craftRandomMessage());
		TS_ASSERT_EQUALS(aMessage, aMessage);
	}
};
