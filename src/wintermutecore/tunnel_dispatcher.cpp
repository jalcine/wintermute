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

#include "tunnel_dispatcher.hh"
#include "tunnel.hpp"
#include "logging.hpp"

using Wintermute::Tunnel;
using Wintermute::DispatcherPrivate;
using Wintermute::Events::Emitter;

Tunnel::Dispatcher::Dispatcher(const string& theName) : d_ptr(new DispatcherPrivate)
{
  W_PRV(Dispatcher);
  d->name = theName;
  d->emitter = make_shared<Events::Emitter>(Tunnel::instance()->emitter()->loop());
  wdebug("Built a new dispatcher for the tunnel called " + name() + ".");
}

string Tunnel::Dispatcher::name() const {
  W_PRV(const Dispatcher);
  return d->name;
}

Emitter::Ptr Tunnel::Dispatcher::emitter() const
{
  W_PRV(Dispatcher);
  return d->emitter;
}

Tunnel::Dispatcher::~Dispatcher()
{
  wdebug("Destroyed the " + name() + " dispatcher.");
}

