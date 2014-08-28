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

#include <string>
#include "libwintermutecore/tunnel.hpp"
#include "test_suite.hpp"

using std::string;
using Wintermute::Module;
using Wintermute::Message;
using Wintermute::Receiver;
using Wintermute::Dispatcher;
using Wintermute::Tunnel;

Message aGlobalMessageForAll;

class SampleDispatcher : public Dispatcher
{
public:
  virtual inline string name() const
  {
    return "sample";
  }
  virtual inline bool send(const Message& message)
  {
    aGlobalMessageForAll = message;
    Tunnel::addMessageToQueue(message);
    TS_ASSERT_EQUALS ( aGlobalMessageForAll, message );
    return true;
  }
};

class SampleReceiver : public Receiver
{
public:
  virtual inline string name() const
  {
    return "sample";
  }
  virtual inline Message receive()
  {
    return aGlobalMessageForAll;
  }
};

class TunnelTestSuite : public CxxTest::TestSuite
{
public:
  void testSendOutMessage(void)
  {
    Receiver::Ptr receiverPtr(new SampleReceiver);
    Dispatcher::Ptr dispatcherPtr(new SampleDispatcher);
    Message message = craftRandomMessage();

    Tunnel::registerDispatcher(dispatcherPtr);
    Tunnel::registerReceiver(receiverPtr);
    TS_ASSERT ( Tunnel::sendMessage(message) );

    Message obtainedMessage = receiverPtr->receive();

    TS_ASSERT ( !!obtainedMessage );
    TS_ASSERT_EQUALS ( message.isLocal(), obtainedMessage.isLocal() );
    TS_ASSERT_EQUALS ( message, obtainedMessage );
  }

  void testReceiveMessage(void)
  {
    Receiver::Ptr receiverPtr(new SampleReceiver);
    Dispatcher::Ptr dispatcherPtr(new SampleDispatcher);
    const Message message = craftRandomMessage();

    Tunnel::registerDispatcher(dispatcherPtr);
    Tunnel::registerReceiver(receiverPtr);
    TS_ASSERT ( Tunnel::sendMessage(message) );
    const Message receivedMessage = Tunnel::receiveMessage();
    TS_ASSERT ( !!receivedMessage );

    TS_ASSERT_EQUALS(receivedMessage, message);
  }

  void testFindADispatcher(void)
  {
    Dispatcher::Ptr dispatcherPtr(new SampleDispatcher);
    TS_ASSERT(Tunnel::registerDispatcher(dispatcherPtr));
    TS_ASSERT(Tunnel::knowsOfDispatcher(dispatcherPtr->name()));
    TS_ASSERT(!Tunnel::knowsOfDispatcher("foobarzilla"));
  }

  void testAddAndFindAReceiver(void)
  {
    Receiver::Ptr receiverPtr(new SampleReceiver);
    TS_ASSERT(Tunnel::registerReceiver(receiverPtr));
    TS_ASSERT(Tunnel::knowsOfReceiver(receiverPtr->name()));
    TS_ASSERT(!Tunnel::knowsOfReceiver("foobarzilla"));
  }
};
