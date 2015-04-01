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
#include <algorithm>
#include <wintermute-core/logging.hpp>
#include "globals.hpp"
#include "dispatcher.hpp"

using std::to_string;
using std::for_each;
using Wintermute::ZMQDispatcher;

ZMQDispatcher::ZMQDispatcher(const SharedPtr<zmqpp::context>& theContext) :
  Dispatcher(WINTERMUTE_ZMQ_TUNNEL_NAME),
  context(theContext),
  socketPtr(nullptr)
{
  wdebug("Building the ZeroMQ dispatcher...");
  assert(theContext);
  wdebug("Built the ZeroMQ dispatcher.");
}

void ZMQDispatcher::bindTo(const string& bindStr)
{
  wdebug("Binding to '" + bindStr + "'...");
  socketPtr->bind( bindStr );
  wdebug("Binded to '" + bindStr + "' via ZeroMQ.");
}

ZMQDispatcher::~ZMQDispatcher()
{
  wdebug("Destroyed this ZeroMQ dispatcher.");
}

bool ZMQDispatcher::send(const Message& message)
{
  assert(!message.isEmpty());
  assert(socketPtr);

  if (message.isEmpty())
  {
    wwarn("Attempt to send empty message thwarted.");
    return false;
  }

  const string msgStr = static_cast<string>(message);
  assert(!msgStr.empty());

  wdebug("Sending message " + msgStr + " over ZeroMQ...");
  const bool socketSentMessage = socketPtr->send(msgStr,
    zmqpp::socket::dont_wait);
  wdebug("Was message sent over ZeroMQ successfully? " +
    to_string(socketSentMessage));

  return socketSentMessage;
}

list<string> ZMQDispatcher::clients() const
{
  list<string> clientList;
  clientList.push_back(string("tcp://0.0.0.0:") +
    to_string(WINTERMUTE_ZMQ_PORT));
  clientList.push_back(string("ipc://") +
    WINTERMUTE_ZMQ_IPC_ADDRESS);

  return clientList;
}

void ZMQDispatcher::start()
{
  auto socketType = zmqpp::socket_type::publish;
  socketPtr = make_shared<zmqpp::socket>(*context, socketType);
  assert(*socketPtr);
  list<string> clientList = clients();
  auto memFunc = std::mem_fn(&ZMQDispatcher::bindTo);
  auto bindFunc = std::bind(memFunc, this, std::placeholders::_1);
  for_each(clientList.begin(), clientList.end(), bindFunc);
}

void ZMQDispatcher::stop()
{
  socketPtr->close();
  socketPtr.reset();
  socketPtr = nullptr;
}
