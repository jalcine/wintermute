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

#ifndef WINTERMUTE_ZMQ_RECEIVER_HPP_
# define WINTERMUTE_ZMQ_RECEIVER_HPP_

#include <wintermute-core/tunnel.hpp>
#include <wintermute-core/events.hpp>
#include <zmqpp/zmqpp.hpp>

namespace Wintermute
{
class ZMQReceiver : public Tunnel::Receiver
{
private:
  SharedPtr<zmqpp::context> context;
  SharedPtr<zmqpp::socket> socketPtr;
  Events::Poller::Ptr pollerPtr;
  Events::Listener::Ptr listenerReceived;

  list<string> clients();
  void connectToClients();
  void disconnectFromClients();
  void attachPoller();
  void attachPollerToSocket();
  void detachPoller();
  void subscribeTo(const string& subscriptionStr);
  void unsubscribeFrom(const string& subscriptionStr);
  void connectTo(const string& connectionStr);
  void disconnectFrom(const string& connectionStr);

public:
  W_DECL_PTR_TYPE(ZMQReceiver)
  explicit ZMQReceiver(const SharedPtr<zmqpp::context>& context);
  virtual ~ZMQReceiver();
  virtual void start();
  virtual void stop();
};
}

#endif
