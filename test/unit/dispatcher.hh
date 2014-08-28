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
#include "libwintermutecore/dispatcher.hpp"
#include "libwintermutecore/message.hpp"

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
