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

#include "library.hpp"
#include "plugin.hpp"
#include "plugin.hh"
#include "logging.hpp"

using Wintermute::Plugin;
using Wintermute::PluginPrivate;

Plugin::Plugin(const string& pluginName) : d_ptr(new PluginPrivate(pluginName))
{
  wdebug("Allocated a plugin named " + pluginName + ".");
}

Plugin::~Plugin()
{
  wdebug("Deallocated a plugin.");
}

Plugin::Ptr Plugin::find(const string& pluginQuery)
{
  wdebug("Searching for a plugin identified by " + pluginQuery + " ...");
  if (pluginQuery.empty())
  {
    wwarn("Provided empty plugin search query.");
    return nullptr;
  }

  Library::Ptr pluginLibrary(Library::find(pluginQuery));

  if (!pluginLibrary)
  {
    wwarn("Failed to find the library to use to handle the plugin '" + pluginQuery + "' with.");
    return nullptr;
  }

  PluginPrivate::CtorFunctionPtr ctorFunction;
  W_RESOLVE_FUNCTION(ctorFunction,
                     pluginLibrary->resolveFunction(WINTERMUTE_PLUGIN_CTOR_FUNCTION_NAME));

  if (!ctorFunction)
  {
    wwarn("Failed to resolve function for plugin ctor.");
    return nullptr;
  }

  Plugin::Ptr pluginPtr;
  ctorFunction(pluginPtr);

  if (!pluginPtr || pluginPtr == nullptr)
  {
    werror("Failed to create a instance of the plugin.");
    return nullptr;
  }

  pluginPtr->d_func()->library = pluginLibrary;
  PluginPrivate::registerPlugin(pluginPtr);

  return pluginPtr;
}

bool Plugin::release(const string& pluginName)
{
  if (Plugin::hasPlugin(pluginName))
  {
    auto pluginLookupItr = PluginPrivate::plugins.find(pluginName);
    const string name(pluginLookupItr->first);
    Plugin::Ptr plugin(pluginLookupItr->second);
    auto library = plugin->d_func()->library;
    wdebug("Unloading plugin " + name + "...");

    PluginPrivate::DtorFunctionPtr dtorFunction;
    W_RESOLVE_FUNCTION(dtorFunction, library->resolveFunction(WINTERMUTE_PLUGIN_DTOR_FUNCTION_NAME));

    const bool freedPlugin = dtorFunction(plugin);

    if (!freedPlugin) {
      werror("Failed to free plugin " + name + " from memory.");
    }
  }

  return false;
}

bool Plugin::hasPlugin(const string& pluginName)
{
  return PluginPrivate::plugins.find(pluginName) != std::end(PluginPrivate::plugins);
}

string Plugin::name() const
{
  W_PRV(const Plugin);
  return d->name;
}
