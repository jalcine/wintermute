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

#ifndef WINTERMUTE_ZMQ_DISPATCHER_HPP_
# define WINTERMUTE_ZMQ_DISPATCHER_HPP_

#include <wintermutecore/tunnel.hpp>
#include <zmqpp/zmqpp.hpp>
#include "globals.hpp"

namespace Wintermute
{
class ZMQDispatcher : public Tunnel::Dispatcher
{
private:
  SharedPtr<zmqpp::context_t> context;
  SharedPtr<zmqpp::socket_t> socketPtr;
  void bindTo(const string& bindStr);
  list<string> clients() const;

public:
  W_DECL_PTR_TYPE(ZMQDispatcher)
  explicit ZMQDispatcher(const SharedPtr<zmqpp::context_t>& context);
  virtual ~ZMQDispatcher();
  virtual bool send(const Message& message) final;
  virtual void start();
  virtual void stop();
};
}

#endif
