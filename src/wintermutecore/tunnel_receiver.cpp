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

#include "tunnel_receiver.hh"
#include "tunnel.hpp"
#include "logging.hpp"

using Wintermute::Tunnel;
using Wintermute::ReceiverPrivate;
using Wintermute::Events::Emitter;
using Wintermute::Events::Event;
using Wintermute::Events::Loop;

Tunnel::Receiver::Receiver(const string& theName) :
  d_ptr(new ReceiverPrivate)
{
  W_PRV(Receiver);
  d->name = theName;
  Loop::Ptr loop = Tunnel::instance()->emitter()->loop();
  assert(loop);
  d->emitter = make_shared<Events::Emitter>(loop);

  auto cb = [](const Event::Ptr& eventPtr)
  {
    const Tunnel::MessageEvent::Ptr msgEvent =
      std::dynamic_pointer_cast<Tunnel::MessageEvent>(eventPtr);

    if (!msgEvent) return; // We were not meant to deal with this.

    if (msgEvent->direction == Tunnel::MessageEvent::DirectionIncoming)
    {
      wdebug("Incoming message: " + (string) msgEvent->message);
      Tunnel::instance()->emitEvent(msgEvent);
    }
  };

  assert ( listenForEvent(W_EVENT_TUNNEL_MESSAGE, cb) );
  wdebug("Built a new receiver for the tunnel called '" + name() + "'.");
}

Tunnel::Receiver::~Receiver()
{
  wdebug("Destroyed the " + name() + " receiver.");
}

string Tunnel::Receiver::name() const
{
  W_PRV(const Receiver);
  return d->name;
}

Emitter::Ptr Tunnel::Receiver::emitter() const
{
  W_PRV(Receiver);
  return d->emitter;
}

void Tunnel::Receiver::handleMessage(const Message& message)
{
  wdebug("Obtained " + static_cast<string>(message) +
    "; raising in Tunnel...");
  MessageEvent::Ptr msgEvent = make_shared<MessageEvent>(message);
  msgEvent->direction = MessageEvent::DirectionIncoming;
  Tunnel::instance()->emitEvent(msgEvent);
  wdebug("Raised Message in Tunnel.");
}
