#include <string>
#include <list>
#include <cxxtest/TestSuite.h>
#include "libwintermutecore/tunnel.hpp"
#include "libwintermutecore/dispatcher.hpp"
#include "libwintermutecore/receiver.hpp"
#include "libwintermutecore/module.hpp"
#include "includes.hpp"

using std::string;
using std::list;
using Wintermute::Module;
using Wintermute::Message;
using Wintermute::Receiver;
using Wintermute::Dispatcher;
using Wintermute::Tunnel;

Message aGlobalMessageForAll;

class SampleDispatcher : public Dispatcher
{
public:
  virtual string name() const
  {
    return "sample";
  }
  virtual bool send(const Message& message)
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
  virtual string name() const
  {
    return "sample";
  }
  virtual Message receive()
  {
    return aGlobalMessageForAll;
  };
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
    TS_ASSERT( Tunnel::sendMessage(message) );

    Message obtainedMessage = receiverPtr->receive();

    TS_ASSERT( !!obtainedMessage );
    TS_ASSERT_EQUALS( message.isLocal(), obtainedMessage.isLocal() );
    TS_ASSERT_EQUALS( message, obtainedMessage );
  }

  void testReceiveMessage(void)
  {
    Receiver::Ptr receiverPtr(new SampleReceiver);
    Dispatcher::Ptr dispatcherPtr(new SampleDispatcher);
    const Message message = craftRandomMessage();

    Tunnel::registerDispatcher(dispatcherPtr);
    Tunnel::registerReceiver(receiverPtr);
    TS_ASSERT (Tunnel::sendMessage(message));
    const Message receivedMessage = Tunnel::receiveMessage();
    TS_ASSERT (!!receivedMessage);

    TS_ASSERT_EQUALS(receivedMessage, message);
  }

  void testAddAndFindADispatcher(void)
  {
    Dispatcher::Ptr dispatcherPtr(new SampleDispatcher);
    TS_ASSERT(Tunnel::registerDispatcher(dispatcherPtr));
    TS_ASSERT(Tunnel::knowsOfDispatcher(dispatcherPtr->name()));
  }

  void testAddAndFindAReceiver(void)
  {
    Receiver::Ptr receiverPtr(new SampleReceiver);
    TS_ASSERT(Tunnel::registerReceiver(receiverPtr));
    TS_ASSERT(Tunnel::knowsOfReceiver(receiverPtr->name()));
  }
};
