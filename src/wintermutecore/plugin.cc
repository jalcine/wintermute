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

#include "plugin.hpp"
#include "logging.hpp"
#include "plugin.hh"

using std::to_string;
using Wintermute::Plugin;
using Wintermute::PluginPrivate;

PluginPrivate::PluginMap PluginPrivate::plugins;

PluginPrivate::PluginPrivate(const string& pluginName) :
  library(), name(pluginName)
{
  wdebug("Defined the internals for the plugin " + pluginName + ".");
}

PluginPrivate::~PluginPrivate()
{
}

bool PluginPrivate::registerPlugin(Plugin::Ptr& plugin)
{
  wdebug("Inserted " + plugin->name() + " into the pool.");
  auto insertionStatus = PluginPrivate::plugins.emplace(plugin->name(), plugin);
  wdebug("Was the plugin " + plugin->name() + " inserted? "
    + to_string(insertionStatus.second));
  return insertionStatus.second;
}

bool PluginPrivate::unregisterPlugin(const string& pluginName)
{
  wdebug("Removed " + pluginName + " from the pool.");
  const auto erasedCount = PluginPrivate::plugins.erase(pluginName);
  wdebug("Was the plugin " + pluginName + " removed? "
    + to_string(erasedCount));
  return erasedCount == 1;
}
