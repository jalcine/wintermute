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
using Wintermute::Events::Loop;
using Wintermute::Events::Event;
using std::dynamic_pointer_cast;

W_DECLARE_SINGLETON(Tunnel)

Tunnel::Tunnel() : d_ptr(new TunnelPrivate)
{
  Tunnel::d_ptr->emitter = make_shared<Emitter>(Loop::primary());
  // TODO: Add listener for incoming messages.
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

  // TODO: Move this into the constructor for the Dispatcher.
  Tunnel::instance()->listenForEvent(W_EVENT_TUNNEL_MESSAGE,
  [&dispatcher](const Event::Ptr& event)
  {
    Tunnel::MessageEvent::Ptr msgEvent =
      dynamic_pointer_cast<Tunnel::MessageEvent>(event);

    if (!msgEvent) return; // We were not meant to deal with this.

    if (msgEvent->direction == Tunnel::MessageEvent::DirectionOutgoing)
    {
      const Message message = msgEvent->message;
      const string name = dispatcher->name();
      const bool wasSent = dispatcher->send(message);
      if (!wasSent)
      {
        werror("Failed to send out a message using the '" + name + "' dispatcher!");
      }
    }
  });

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

  // TODO: Move this into the constructor for the Receiver.
  receiver->listenForEvent("core.tunnel.message", [](const Events::Event::Ptr& eventPtr)
  {
    const Tunnel::MessageEvent::Ptr msgEvent =
      std::dynamic_pointer_cast<Tunnel::MessageEvent>(eventPtr);

    if (!msgEvent) return; // We were not meant to deal with this.

    if (msgEvent->direction == Tunnel::MessageEvent::DirectionIncoming)
    {
      wdebug(msgEvent->message);
      Tunnel::instance()->emitEvent(msgEvent);
    }
  });

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

void Tunnel::sendMessage(const Message& message)
{
  wdebug("Queuing message '" + static_cast<string>(message) + "' for delivery...");
  Tunnel::MessageEvent::Ptr msgPtr = make_shared<Tunnel::MessageEvent>(message);
  msgPtr->direction = Tunnel::MessageEvent::DirectionOutgoing;
  instance()->emitEvent(msgPtr);
}
