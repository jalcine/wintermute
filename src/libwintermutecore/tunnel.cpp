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

#include <algorithm>
#include "tunnel.hh"
#include "tunnel.hpp"
#include "logging.hpp"

#define dPtr Tunnel::instance()->d_func()

using Wintermute::Dispatcher;
using Wintermute::Receiver;
using Wintermute::Message;
using Wintermute::Tunnel;
using Wintermute::TunnelPrivate;

W_DECLARE_SINGLETON(Tunnel);

Tunnel::Tunnel() : d_ptr(new TunnelPrivate)
{
}

bool Tunnel::registerDispatcher(const Dispatcher::Ptr& dispatcher)
{
  dPtr->dispatchers.insert(std::make_pair(dispatcher->name(), dispatcher));
  return dPtr->dispatchers.find(dispatcher->name()) != dPtr->dispatchers.end();
}

bool Tunnel::unregisterDispatcher(const Dispatcher::Ptr& dispatcher)
{
  return dPtr->dispatchers.erase(dispatcher->name()) == 1;
}

bool Tunnel::registerReceiver(const Receiver::Ptr& receiver)
{
  dPtr->receivers.insert(std::make_pair(receiver->name(), receiver));
  return dPtr->receivers.find(receiver->name()) != dPtr->receivers.end();
}

bool Tunnel::unregisterReceiver(const Receiver::Ptr& receiver)
{
  return dPtr->receivers.erase(receiver->name()) == 1;
}

bool Tunnel::sendMessage(const Message& message)
{
  wdebug("Sending message '" + static_cast<string>(message) + "'...")
  for (auto itr : dPtr->dispatchers)
  {
    const string name = itr.first;
    Dispatcher::Ptr dispatcher = itr.second;
    wdebug("Sending the message using the '" + name + "' dispatcher...");
    if (!dispatcher->send(message))
    {
      werror("Failed to send out a message using the '" + name + "' dispatcher!")
      return false;
    }
  };
  return true;
}

bool Tunnel::hasPendingMessages()
{
  return !dPtr->obtainedMessages.empty();
}

const Message Tunnel::receiveMessage()
{
  if (Tunnel::hasPendingMessages())
  {
    const Message theMessage = dPtr->obtainedMessages.top();
    dPtr->obtainedMessages.pop();
    return theMessage;
  }
  else
  {
    return Message();
  }
}

void Tunnel::addMessageToQueue(const Message& message)
{
  dPtr->obtainedMessages.push(message);
}

bool Tunnel::knowsOfReceiver(const string& receiverName)
{
  return dPtr->receivers.count(receiverName) != 0;
}

bool Tunnel::knowsOfDispatcher(const string& dispatcherName)
{
  return dPtr->dispatchers.count(dispatcherName) != 0;
}

Tunnel::~Tunnel()
{
}

TunnelPrivate::TunnelPrivate()
{
}

TunnelPrivate::~TunnelPrivate()
{
}
