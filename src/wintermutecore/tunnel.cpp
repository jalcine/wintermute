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
using std::for_each;
using std::begin;
using std::end;
using std::make_pair;

// TODO: Swap 'insert' for 'emplace'.

W_DECLARE_SINGLETON(Tunnel)

Tunnel::Tunnel() :
  d_ptr(make_shared<TunnelPrivate>())
{
  assert(d_ptr);
}

Tunnel::~Tunnel()
{
  if (Tunnel::_instance)
  {
    d_ptr.reset();
  }
}

Emitter::Ptr Tunnel::emitter() const
{
  W_SPRV(Tunnel);
  if (d->emitter == nullptr)
  {
    d->emitter = make_shared<Emitter>();
  }

  assert(d->emitter);
  return d->emitter;
}

bool Tunnel::registerDispatcher(const Tunnel::Dispatcher::Ptr& dispatcher)
{
  assert(dispatcher);
  W_SPRV(Tunnel);
  auto thePair = make_pair(dispatcher->name(), dispatcher);
  d->dispatchers.insert(thePair);
  return !knowsOfDispatcher(dispatcher->name()) == false;
}

bool Tunnel::unregisterDispatcher(const string& dispatcherName)
{
  W_SPRV(Tunnel);
  auto itr = d->dispatchers.find(dispatcherName);
  if (itr == end(d->dispatchers))
  {
    return false;
  }

  Tunnel::Dispatcher::Ptr dispatcherPtr;
  dispatcherPtr = d->dispatchers.find(dispatcherName)->second;
  assert(dispatcherPtr);
  return unregisterDispatcher(dispatcherPtr);
}

bool Tunnel::unregisterDispatcher(const string& dispatcherName)
{
  W_SPRV(Tunnel);
  auto itr = d->dispatchers.find(dispatcherName);
  if (itr == std::end(d->dispatchers))
  {
    return false;
  }

  Tunnel::Dispatcher::Ptr dispatcherPtr = d->dispatchers.find(dispatcherName)->second;
  return unregisterDispatcher(dispatcherPtr);
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
  assert(receiver);
  W_SPRV(Tunnel);
  auto thePair = make_pair(receiver->name(), receiver);
  d->receivers.insert(thePair);
  return knowsOfReceiver(receiver->name());
}

bool Tunnel::unregisterReceiver(const string& receiverName)
{
  W_SPRV(Tunnel);
  auto itr = d->receivers.find(receiverName);
  if (itr == std::end(d->receivers))
  {
    return false;
  }

  Tunnel::Receiver::Ptr receiverPtr = d->receivers.find(receiverName)->second;
  return unregisterReceiver(receiverPtr);
}

bool Tunnel::unregisterReceiver(const Tunnel::Receiver::Ptr& receiver)
{
  W_SPRV(Tunnel);
  if (receiver)
  {
    auto count = d->receivers.erase(receiver->name()) == 1;
    return count == 1;
  }

  return false;
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
  wdebug("Queuing message '" + static_cast<string>(message) +
    "' for delivery...");

  Tunnel::MessageEvent::Ptr msgPtr(nullptr);
  msgPtr = make_shared<Tunnel::MessageEvent>(message);
  msgPtr->direction = Tunnel::MessageEvent::DirectionOutgoing;
  instance()->emitEvent(msgPtr);

  wdebug("Message queued.");
}

void Tunnel::start()
{
  W_SPRV(Tunnel);
  auto startDispatcherFunc =
    [](std::pair<const string, Tunnel::Dispatcher::Ptr> pair)
  {
    winfo("Starting the " + pair.second->name() + " dispatcher...");
    assert(pair.second);
    pair.second->start();
    winfo("Started the " + pair.second->name() + " dispatcher.");
  };
  auto startReceiverFunc =
    [](std::pair<const string, Tunnel::Receiver::Ptr> pair)
  {
    winfo("Starting the " + pair.second->name() + " receiver...");
    assert(pair.second);
    pair.second->start();
    winfo("Started the " + pair.second->name() + " receiver.");
  };

  wdebug("Starting the Tunnel...");
  for_each(d->receivers.begin(), d->receivers.end(), startReceiverFunc);
  for_each(d->dispatchers.begin(), d->dispatchers.end(), startDispatcherFunc);
  wdebug("Started the Tunnel.");
}

void Tunnel::stop()
{
  auto d = Tunnel::_instance->d_func();

  auto stopDispatcherFunc =
    [](std::pair<const string, Tunnel::Dispatcher::Ptr> pair)
  {
    winfo("Stopping the " + pair.second->name() + " dispatcher...");
    assert(pair.second);
    pair.second->stop();
    winfo("Stopped the " + pair.second->name() + " dispatcher.");
  };
  auto stopReceiverFunc =
    [](std::pair<const string, Tunnel::Receiver::Ptr> pair)
  {
    winfo("Stopping the " + pair.second->name() + " receiver...");
    assert(pair.second);
    pair.second->stop();
    winfo("Stopped the " + pair.second->name() + " receiver.");
  };

  wdebug("Stopping the Tunnel...");
  for_each(d->dispatchers.begin(), d->dispatchers.end(), stopDispatcherFunc);
  for_each(d->receivers.begin(), d->receivers.end(), stopReceiverFunc);
  wdebug("Stopped the Tunnel.");
}
