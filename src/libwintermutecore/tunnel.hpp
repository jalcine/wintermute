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

#ifndef WINTERMUTE_CORE_TUNNEL_HPP
#define WINTERMUTE_CORE_TUNNEL_HPP

#include <list>
#include "globals.hpp"
#include "dispatcher.hpp"
#include "receiver.hpp"
#include "message.hpp"

using std::list;

namespace Wintermute
{
class Message;
class TunnelPrivate;
/* Handles the act of sending and receiving new messages.
 * The Tunnel serves as a bridge between the Dispatcher, Receiver and
 * ModulePool singleton to transfer a message from one place to another
 * transparently. The Tunnel works closely with MessageSerializer to
 * convert incoming data into a Message and outgoing Message objects into raw
 * data.
 */
class Tunnel
{
  friend class Receiver;
  W_DEFINE_PRIVATE(Tunnel)
  W_DEFINE_SINGLETON(Tunnel)

  explicit Tunnel();
  virtual ~Tunnel();

  static void addMessageToQueue(const Message& message);
public:
  // Registers a dispatcher into the Tunnel.
  static bool registerDispatcher(const Dispatcher::Ptr& dispatcher);

  // Registers a receiver into the Tunnel.
  static bool registerReceiver(const Receiver::Ptr& receiver);

  // Unregisters a dispatcher into the Tunnel.
  static bool unregisterDispatcher(const Dispatcher::Ptr& dispatcher);

  // Unregisters a receiver into the Tunnel.
  static bool unregisterReceiver(const Receiver::Ptr& receiver);

  static bool sendMessage(const Message& message);
  static bool hasPendingMessages();
  static const Message receiveMessage();

  static bool knowsOfDispatcher(const string& dispatcherName);
  static bool knowsOfReceiver(const string& receiverName);
};
}

#endif
