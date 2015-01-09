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

using Wintermute::Message;
using Wintermute::Tunnel;
using Wintermute::TunnelPrivate;
using Wintermute::Events::Emitter;

W_DECLARE_SINGLETON(Tunnel)

Tunnel::Tunnel() : d_ptr(new TunnelPrivate)
{
}

Tunnel::~Tunnel()
{
  //clearAllReceivers();
  //clearAllDispatchers();
}

Emitter::Ptr Tunnel::emitter() const
{
  W_SPRV(Tunnel);
  return d->emitter;
}

bool Tunnel::registerDispatcher(const Tunnel::Dispatcher::Ptr& dispatcher)
{
  W_SPRV(Tunnel);
  auto thePair = std::make_pair(dispatcher->name(), dispatcher);
  d->dispatchers.insert(thePair);

  return !knowsOfDispatcher(dispatcher->name()) == false;
}


bool Tunnel::unregisterDispatcher(const Tunnel::Dispatcher::Ptr& dispatcher)
{
  W_SPRV(Tunnel);
  auto count = d->dispatchers.erase(dispatcher->name());
  return count == 1;
}

void Tunnel::clearAllDispatchers()
{
  W_SPRV(Tunnel);
  d->dispatchers.clear();
}

bool Tunnel::knowsOfDispatcher(const string& dispatcherName)
{
  W_SPRV(Tunnel);
  return d->dispatchers.count(dispatcherName) != 0;
}

Tunnel::Dispatcher::List Tunnel::dispatchers()
{
  W_SPRV(Tunnel);
  Tunnel::Dispatcher::List listOfDispatchers;
  for (auto pair : d->dispatchers)
  {
    listOfDispatchers.push_back(pair.second);
  }

  return listOfDispatchers;
}

bool Tunnel::registerReceiver(const Tunnel::Receiver::Ptr& receiver)
{
  W_SPRV(Tunnel);
  auto thePair = std::make_pair(receiver->name(), receiver);
  d->receivers.insert(thePair);

  return !knowsOfReceiver(receiver->name()) == false;
}

bool Tunnel::unregisterReceiver(const Tunnel::Receiver::Ptr& receiver)
{
  W_SPRV(Tunnel);
  auto count = d->receivers.erase(receiver->name()) == 1;
  return count == 1;
}

void Tunnel::clearAllReceivers()
{
  W_SPRV(Tunnel);
  d->receivers.clear();
}

bool Tunnel::knowsOfReceiver(const string& receiverName)
{
  W_SPRV(Tunnel);
  return d->receivers.count(receiverName) != 0;
}

Tunnel::Receiver::List Tunnel::receivers()
{
  W_SPRV(Tunnel);
  Tunnel::Receiver::List listOfReceivers;
  for (auto pair : d->receivers)
  {
    listOfReceivers.push_back(pair.second);
  }

  return listOfReceivers;
}

bool Tunnel::sendMessage(const Message& message)
{
  wdebug("Sending message '" + static_cast<string>(message) + "'...");
  Dispatcher::List dispatchers = Tunnel::instance()->dispatchers();
  auto sendAMessage = [&message](Dispatcher::Ptr & dispatcherPtr)
  {
    const string name = dispatcherPtr->name();
    const bool wasSent = dispatcherPtr->send(message);
    if (!wasSent)
    {
      werror("Failed to send out a message using the '" + name + "' dispatcher!");
    }

    return wasSent;
  };


  for_each(dispatchers.begin(), dispatchers.end(), sendAMessage);
  return true;
}

bool Tunnel::hasPendingMessages()
{
  W_SPRV(Tunnel);
  return !d->obtainedMessages.empty();
}

const Message Tunnel::receiveMessage()
{
  W_SPRV(Tunnel);
  if (Tunnel::hasPendingMessages())
  {
    const Message theMessage = d->obtainedMessages.top();
    d->obtainedMessages.pop();
    return theMessage;
  }
  else
  {
    return Message();
  }
}


