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

#include <wintermute-core/logging.hpp>
#include "globals.hpp"
#include "plugin.hpp"
#include "dispatcher.hpp"
#include "receiver.hpp"

using Wintermute::Plugin;
using Wintermute::ZMQPlugin;
using Wintermute::ZMQDispatcher;
using Wintermute::ZMQReceiver;

ZMQPlugin::ZMQPlugin() :
  Plugin(WINTERMUTE_ZMQ_PLUGIN_NAME),
  context(nullptr),
  receiver(nullptr),
  dispatcher(nullptr)
{
  assert(!receiver);
  assert(!dispatcher);

  context = make_shared<zmqpp::context>();
  assert(context);
}

ZMQPlugin::~ZMQPlugin()
{
  wdebug("Killed the ZeroMQ plugin.");
}

bool ZMQPlugin::startup()
{
  wdebug("ZeroMQ plugin starting...");
  if (!receiver)
  {
    ZMQReceiver* r = new ZMQReceiver(context);
    receiver.reset(r);
    assert(receiver.unique());
  }

  if (!dispatcher)
  {
    dispatcher = make_shared<ZMQDispatcher>(context);
    assert(dispatcher);
  }

  assert(Tunnel::registerReceiver(receiver));
  assert(Tunnel::registerDispatcher(dispatcher));
  wdebug("ZeroMQ plugin started.");
  return true;
}

bool ZMQPlugin::shutdown()
{
  wdebug("ZeroMQ plugin shutting down...");
  assert(Tunnel::unregisterDispatcher(dispatcher));
  assert(Tunnel::unregisterReceiver(receiver));
  receiver.reset();
  dispatcher.reset();
  wdebug("ZeroMQ plugin shut down.");
  return true;
}

Plugin::PluginType ZMQPlugin::type() const
{
  return Plugin::PluginTypeSupport;
}
