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

#include "logging.hpp"
#include "tunnel.hh"

using Wintermute::TunnelPrivate;
using Wintermute::Events::Emitter;
using Wintermute::Events::Loop;

TunnelPrivate::TunnelPrivate() :
  dispatchers(),
  receivers(),
  emitter(nullptr)
{
  emitter = make_shared<Emitter>();
  wdebug("Built up Tunnel's private data.");
}

TunnelPrivate::~TunnelPrivate()
{
  // TODO: Free up all of the dispatchers, if not automatically.
  // TODO: Free up all of the receivers, if not automatically.
}
