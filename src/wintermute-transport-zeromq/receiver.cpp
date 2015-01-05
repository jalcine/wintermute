/*
    Wintermute - A foundation for intelligent computing.
    Copyright (c) 2010 - 2015 by Jacky Alcine

    Wintermute is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    Wintermute is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with Wintermute; if not, write to the
    Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
 */

#include <string>
#include <algorithm>
#include <limits>
#include <wintermutecore/util/serializable.hpp>
#include <wintermutecore/logging.hpp>
#include "globals.hpp"
#include "receiver.hpp"

using Wintermute::Message;
using Wintermute::ZMQReceiver;
using Wintermute::Util::Serializable;
using Wintermute::Events::Event;
using Wintermute::Events::Poller;
using Wintermute::Events::PollEvent;
using Wintermute::Tunnel;
using std::to_string;
using std::for_each;
using std::dynamic_pointer_cast;
using std::placeholders::_1;

ZMQReceiver::ZMQReceiver(const SharedPtr<zmqpp::context>& theContext) :
  Receiver(WINTERMUTE_ZMQ_TUNNEL_NAME),
  context(theContext),
  socketPtr(nullptr),
  pollerPtr(nullptr),
  listenerReceived(nullptr)
{
  wdebug("ZMQReceiver building...");
  if (!context)
  {
    throw std::invalid_argument(
      "ZMQReceiver: Need a pointer to a ZeroMQ context.");
  }
  wdebug("ZMQReceiver built.");
}

ZMQReceiver::~ZMQReceiver()
{
  //stop();
  wdebug("Destroyed this ZeroMQ receiver.");
}

void ZMQReceiver::start()
{
  connectToClients();
  attachPoller();
}

void ZMQReceiver::stop()
{
  detachPoller();
  disconnectFromClients();
}

list<string> ZMQReceiver::clients()
{
  list<string> clientList;
  clientList.push_back("tcp://0.0.0.0:" + to_string(WINTERMUTE_ZMQ_PORT));
  clientList.push_back("ipc://" WINTERMUTE_ZMQ_IPC_ADDRESS);

  return clientList;
}

void ZMQReceiver::connectToClients()
{
  assert(this);
  auto socketType = zmqpp::socket_type::subscribe;
  socketPtr = make_shared<zmqpp::socket>(*context, socketType);
  assert(socketPtr);

  subscribeTo ( WINTERMUTE_ZMQ_FILTER );
  list<string> clientList = clients();
  auto connectFunc = std::mem_fn(&ZMQReceiver::connectTo);
  auto bindFunc = std::bind(connectFunc, this, _1);
  for_each(clientList.begin(), clientList.end(), bindFunc);
}

void ZMQReceiver::disconnectFromClients()
{
  list<string> clientList = clients();
  auto disconnectFunc = std::mem_fn(&ZMQReceiver::disconnectFrom);
  auto bindFunc = std::bind(disconnectFunc, this, _1);
  for_each(clientList.begin(), clientList.end(), bindFunc);
}

void ZMQReceiver::attachPoller()
{
  assert(socketPtr);

  int fd = 0;
  socketPtr->get(zmqpp::socket_option::file_descriptor, fd);
  assert(fd <= std::numeric_limits<int>::max());

  wdebug("Setting up poller for ZeroMQ's file descriptor " + to_string(fd)
    + " on read-only mode...");

  pollerPtr = make_shared<Poller>(fd, Poller::PollReadable);
  assert(pollerPtr);
  attachPollerToSocket();

  assert ( pollerPtr->start() );
}

void ZMQReceiver::attachPollerToSocket()
{
  Events::Listener::Callback cb =
  [this](const Event::Ptr& event) -> void
  {
    assert(event);

    PollEvent::Ptr pollEvent;
    bool wasMessageFound = false;
    pollEvent = std::dynamic_pointer_cast<PollEvent>(event);

    if (!pollEvent)
    {
      wdebug("Not a Poll event.");
      return; // We were not meant to deal with this.
    }

    Message fullMsg;
    zmqpp::message zmqMsg;
    auto socketPtrRef = this->socketPtr;
    wdebug("Obtained a poll request from the ZeroMQ socket.");

    try
    {
      wasMessageFound = socketPtrRef->receive(zmqMsg, true);
    }
    catch (zmqpp::zmq_internal_exception& e)
    {
      wdebug(string("Couldn't poll from ZeroMQ socket: ") + e.what());
    }

    if (wasMessageFound)
    {
      wdebug("This message has " + to_string(zmqMsg.parts()) + " parts.");
      // TODO: Handle import of message parts.
      const string msgStr = zmqMsg.get<string>(0);
      wdebug("Received over ZeroMQ: " + msgStr);
      fullMsg = msgStr;
      handleMessage(fullMsg);
    }
    else
    {
      wdebug("Socket polled, but no message found for ZMQReceiver.");
    }
  };

  listenerReceived = pollerPtr->listenForEvent(W_EVENT_POLLED, cb);
  assert ( listenerReceived );
}

void ZMQReceiver::detachPoller()
{
  if (pollerPtr)
  {
    pollerPtr->stop();
    pollerPtr.reset();
  }

  if (listenerReceived)
  {
    listenerReceived.reset();
  }
}

void ZMQReceiver::subscribeTo(const string& subscriptionStr)
{
  if (socketPtr)
  {
    socketPtr->set( zmqpp::socket_option::subscribe, subscriptionStr );
    wdebug("Subscribed to messages with '" + subscriptionStr + "' filter.");
  }
  else
  {
    wdebug("Can't subscribe to '" +
      subscriptionStr + "' since socket is invalid.");
  }
}

void ZMQReceiver::connectTo(const string& connectionStr)
{
  if (socketPtr)
  {
    wdebug("Connecting to '" + connectionStr + "'...");
    socketPtr->connect( connectionStr );
    wdebug("Connected to '" + connectionStr + "' via ZeroMQ.");
  }
  else
  {
    wdebug("Can't connect to '" +
      connectionStr + "' since socket is invalid.");
  }
}

void ZMQReceiver::disconnectFrom(const string& connectionStr)
{
  if (socketPtr)
  {
    wdebug("Disconnecting from '" + connectionStr + "'...");
    socketPtr->disconnect( connectionStr );
    wdebug("Disconnected from '" + connectionStr + "' via ZeroMQ.");
  }
  else
  {
    wdebug("Can't disconnect from '" +
      connectionStr + "' since socket is invalid.");
  }
}

void ZMQReceiver::unsubscribeFrom(const string& subscriptionStr)
{
  if (socketPtr)
  {
    socketPtr->set( zmqpp::socket_option::unsubscribe, subscriptionStr );
    wdebug("Unsubscribed from messages with '" + subscriptionStr + "' filter.");
  }
  else
  {
    wdebug("Can't unsubscribe from '" +
      subscriptionStr + "' since socket is invalid.");
  }
}
