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
#include <iostream>

using Wintermute::Version;
using Wintermute::Library;
using Wintermute::Plugin;
using Wintermute::PluginPrivate;

bool isLibraryCompatible(Library::Ptr& libraryPtr)
{
  assert(libraryPtr);
  if (!libraryPtr)
  {
    werror("Obtained an invalid pointer to a library.");
    //throw std::invalid_argument("Invalid pointer to library.");
    return false;
  }

  PluginPrivate::VersionFunctionPtr versionFunction;
  W_RESOLVE_FUNCTION(versionFunction,
    libraryPtr->resolveFunction(WINTERMUTE_PLUGIN_VERSION_FUNCTION_NAME));

  if (!versionFunction)
  {
    wwarn("Failed to resolve function for plugin library version.");
    return false;
  }

  const string versionString(versionFunction());
  const Version libraryVersion(versionString);
  const Version systemVersion = Version::system();

  wdebug("Raw version string from library: " + versionString);
  wdebug("System " + static_cast<string>(systemVersion) + " >= library min " + static_cast<string>(libraryVersion));
  return systemVersion >= libraryVersion;
}

void loadPluginFromLibrary(Library::Ptr& libraryPtr, Plugin::Ptr& pluginPtr)
{
  PluginPrivate::CtorFunctionPtr ctorFunction;
  W_RESOLVE_FUNCTION(ctorFunction,
    libraryPtr->resolveFunction(WINTERMUTE_PLUGIN_CTOR_FUNCTION_NAME));

  if (!ctorFunction)
  {
    wwarn("Failed to resolve function for plugin ctor.");
    return;
  }

  // TODO: Add exception handling around 'ctorFunction'.
  pluginPtr.reset(ctorFunction());

  if (!pluginPtr)
  {
    werror("Failed to create a instance of the plugin.");
    return;
  }

  pluginPtr->d_func()->library.swap(libraryPtr);
  PluginPrivate::registerPlugin(pluginPtr);
}

Plugin::Plugin(const string& pluginName) : d_ptr(new PluginPrivate(pluginName))
{
  wdebug("Allocated a plugin named " + pluginName + ".");
}

Plugin::~Plugin()
{
  W_PRV(Plugin);

  if (d->library && d->library->loadedStatus() == Library::LoadIsLoaded)
  {
    wdebug("Unloading plugin's library prior to deallocation...");
    d->library->unload();
  }
  else
  {
    wdebug("No library instance.");
  }

  wdebug("Deallocated a plugin.");
}

// TODO: Dice up this function.
Plugin::Ptr Plugin::find(const string& pluginQuery)
{
  Plugin::Ptr pluginPtr = nullptr;

  wdebug("Searching for a plugin identified by " + pluginQuery + " ...");
  if (pluginQuery.empty())
  {
    wdebug("Provided empty plugin search query.");
    return nullptr;
  }

  // TODO: Return pointers to loaded plugins.
  if (hasPlugin(pluginQuery))
  {
    wdebug("Plugin has been loaded before, returning known reference.");
    auto itr = PluginPrivate::plugins.find(pluginQuery);
    assert(itr != std::end(PluginPrivate::plugins));
    assert(itr->second);
    return itr->second;
  }
  else
  {
    wdebug("The plugin is not in the known pool; attempting to load.");
  }

  Library::Ptr pluginLibrary = Library::find(pluginQuery);
  assert(pluginLibrary.unique());

  if (!pluginLibrary)
  {
    wwarn("Failed to find the library to use to handle the plugin " + pluginQuery + " with.");
    return nullptr;
  }

  assert(pluginLibrary);

  if (isLibraryCompatible(pluginLibrary))
  {
    loadPluginFromLibrary(pluginLibrary, pluginPtr);
    if (pluginPtr)
    {
      winfo("Plugin " + pluginQuery + " loaded, starting...");
      pluginPtr->startup();
      winfo("Plugin " + pluginQuery + " started.");
    }
    else
    {
      werror("Failed to start plugin " + pluginQuery + ".");
      pluginLibrary->unload();
      return nullptr;
    }
  }
  else
  {
    wwarn("The plugin " + pluginQuery + " is incompatible with Wintermute.");
    return nullptr;
  }

  return pluginPtr;
}

bool Plugin::release(const string& pluginName)
{
  if (!Plugin::hasPlugin(pluginName))
  {
    wwarn("The plugin " + pluginName + " doesn't exist in this instance of Wintermute.");
    return true;
  }

  auto pluginLookupItr = PluginPrivate::plugins.find(pluginName);
  const string name(pluginLookupItr->first);
  Plugin::Ptr plugin(pluginLookupItr->second);

  plugin->shutdown();

  auto library = plugin->d_func()->library;
  wdebug("Unloading plugin " + name + "...");

  PluginPrivate::DtorFunctionPtr dtorFunction;
  W_RESOLVE_FUNCTION(dtorFunction,
    library->resolveFunction(WINTERMUTE_PLUGIN_DTOR_FUNCTION_NAME));

  const bool freedPlugin = dtorFunction(plugin);
  // NOTE: DO NOT use 'pluginPtr' after this point here.

  if (!freedPlugin)
  {
    werror("Failed to free plugin " + name + " from memory.");
  }
  else
  {
    wdebug("Plugin " + name + " successfully freed.");
  }

  PluginPrivate::unregisterPlugin(pluginName);
  winfo("Plugin " + name + " unloaded.");

  return true;
}

bool Plugin::hasPlugin(const string& pluginName)
{
  auto countOfPlugins = PluginPrivate::plugins.count(pluginName);
  wdebug(std::to_string(countOfPlugins));
  return countOfPlugins != 0;
}

string Plugin::name() const
{
  W_PRV(const Plugin);
  return d->name;
}

const list<string> Plugin::all()
{
  list<string> pluginNameList;

  for ( auto pluginPair : PluginPrivate::plugins )
  {
    pluginNameList.push_back(pluginPair.first);
  }

  return pluginNameList;
}
