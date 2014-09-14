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
#include "globals.hpp"
#include "plugin.hh"
#include <algorithm>

using Wintermute::Plugin;
using Wintermute::PluginPrivate;

Plugin::Plugin(Plugin& plugin) : d_ptr(plugin.d_ptr)
{
  wdebug("Plugin " + plugin.name() + " was copied.");
}

Plugin::Plugin(const string& pluginName) : d_ptr(new PluginPrivate)
{
  W_PRV(Plugin);
  assert ( pluginName.empty() == false );
  d->name = pluginName;
}

string Plugin::name() const
{
  W_PRV ( const Plugin );
  assert ( d->name.empty() == false );
  return d->name;
}

Plugin::LoadState Plugin::start()
{
  W_PRV ( Plugin );
  wtrace("Starting up plugin " + name() + "...");

  if ( state() == Plugin::Loaded )
  {
    winfo("Plugin " + name() + " already loaded; attempt at double-load?");
    return Plugin::Loaded;
  }

  try
  {
    d->loadState = startup() ? Plugin::Loaded : Plugin::LoadingFailed;
  }
  catch ( std::exception& e )
  {
    d->loadState = Plugin::LoadingFailed;
    d->loadFailure = Plugin::FailureABIException;
    wwarn("Failed to load plugin '" + name() + "' due to " + e.what() + ".");
  }

  return d->loadState;
}

Plugin::LoadState Plugin::stop()
{
  wtrace("Shutting down plugin " + name() + "...");
  const string pluginName = name();

  if ( state() == Plugin::Unloaded )
  {
    wdebug("Plugin " + pluginName + "already unloaded.");
    return Plugin::Unloaded;
  }

  wdebug("Stopping plugin " + pluginName + "...");
  // TODO Catch any errors that'd happen here.
  this->shutdown();
  wdebug("Plugin " + pluginName + " stopped.");
  return Plugin::Unloaded;
}

Plugin::Ptr Plugin::loadByName(const string& name)
{
  assert(!name.empty());
  Plugin::Ptr pluginPtr;

  // NOTE: There *has* to be a better way of doing this.
  const list<string> libraryFileNames =
  {
    name,
    name + ".so",
    "lib" + name + ".so",
    "libwintermute" + name + ".so"
  };

  for (string fileName : libraryFileNames)
  {
    wdebug("Testing out the filename " + fileName + " in place of " + name + "...");
    pluginPtr = W_CLAIM_SHARED_PTR(Plugin::loadFromFilepath(fileName));

    if (pluginPtr)
    {
      wdebug(fileName + " works!");
      break;
    }

    assert(pluginPtr);
  }

  return pluginPtr;
}

Plugin::Ptr Plugin::loadFromFilepath(const string& filepath)
{
  assert(!filepath.empty());

  wdebug("Attempting to load library '" + filepath + "' for plugin...");
  Plugin::Library::Ptr libraryPtr = Plugin::Library::find(filepath);
  wdebug("Was library " + filepath + " found on `ld`? " + (libraryPtr ? "YES" : "NO"));

  if (libraryPtr)
  {
    return loadFromLibrary(libraryPtr);
  }

  wwarn("Failed to load plugin " + filepath + ".");
  return nullptr;
}

Plugin::Ptr Plugin::loadFromLibrary(Library::Ptr& libraryPtr)
{
  assert(libraryPtr);

  Plugin::Ptr pluginPtr = { nullptr };
  if (!libraryPtr)
  {
    wwarn("No library provided, so no plugin loaded.");
    return pluginPtr;
  }

  if (libraryPtr && !libraryPtr->isLoaded())
  {
    const bool wasLibraryLoaded = libraryPtr->load();
    assert(wasLibraryLoaded);

    if (!wasLibraryLoaded)
    {
      werror("Failed to load library! " + libraryPtr->errorMessage());
      return pluginPtr;
    }

    wdebug("Loaded library for plugin.");
  }

  assert(libraryPtr);

  wdebug("Resolving the '" + string(WINTERMUTE_PLUGIN_METHOD_CTOR_NAME) + "' function from the library...");
  auto funcCtorHandle = libraryPtr->resolveMethod(WINTERMUTE_PLUGIN_METHOD_CTOR_NAME);
  assert(funcCtorHandle);

  if (!funcCtorHandle)
  {
    werror("Failed to resolve the ctor function for the plugin from the library.");
    return pluginPtr;
  }
  else
  {
    winfo("Resolved ctor for plugin.");
  }

  Plugin::Ptr (*pluginCtorFunction)(void);
  *(void **)(&pluginCtorFunction) = funcCtorHandle;
  assert (pluginCtorFunction);

  try
  {
    winfo("Invoking ctor function for plugin...");
    pluginPtr = W_CLAIM_SHARED_PTR(pluginCtorFunction());
    winfo("Invoked ctor function for plugin.");
  }
  catch (std::exception &e)
  {
    werror(string("Failed to load library for plugin; e: %s") + e.what());
    pluginPtr = nullptr;
    return pluginPtr;
  }

  assert(pluginPtr);
  winfo("Plugin " + pluginPtr->name()  + " successfully loaded!");

  pluginPtr->d_ptr->loadState = Plugin::Loaded;
  pluginPtr->d_ptr->loadFailure = Plugin::FailureNone;
  pluginPtr->d_ptr->libraryPtr = libraryPtr;

  auto pluginPair = std::make_pair(pluginPtr->name(), pluginPtr);
  PluginPrivate::plugins.insert(pluginPair);

  wdebug("Plugin " + pluginPtr->name() + " added to the pool.");

  funcCtorHandle = nullptr;
  return pluginPtr;
}

bool Plugin::isCurrentlyLoaded(const string& name)
{
  auto nameItr = std::find_if(
                   PluginPrivate::plugins.cbegin(),
                   PluginPrivate::plugins.cend(),
                   [&](const Plugin::Map::value_type & pair)
  {
    return pair.first == name;
  });
  auto endOfList = PluginPrivate::plugins.cend();

  return nameItr != endOfList;
}

list<string> Plugin::loadedPlugins()
{
  list<string> pluginList;

  auto citrStart = PluginPrivate::plugins.cbegin();
  auto citrEnd = PluginPrivate::plugins.cend();

  std::for_each(citrStart, citrEnd, [&](const Plugin::Map::value_type & pair)
  {
    pluginList.insert(pluginList.end(), pair.first);
  });

  return pluginList;
}

Plugin::LoadState Plugin::state() const
{
  W_PRV(const Plugin);
  return d->loadState;
}

bool Plugin::unload(const string& name)
{
  wdebug("Attempting to unload plugin " + name + "...");
  auto pluginItr = PluginPrivate::plugins.find(name);

  if (pluginItr == PluginPrivate::plugins.end())
  {
    wdebug("Couldn't find plugin named " + name + ".");
    return false;
  }

  wdebug("Found plugin " + pluginItr->first + " to unload.");
  Plugin::Ptr plugin = W_CLAIM_SHARED_PTR(pluginItr->second);
  auto libraryPtr = W_CLAIM_SHARED_PTR( plugin->d_func()->libraryPtr);
  wdebug("Destroying plugin " + name + "...");

  wdebug("Resolving the '" + string(WINTERMUTE_PLUGIN_METHOD_DTOR_NAME) + "' function from the library...");
  auto funcDtorHandle = libraryPtr->resolveMethod(WINTERMUTE_PLUGIN_METHOD_DTOR_NAME);
  assert(funcDtorHandle);

  if (!funcDtorHandle)
  {
    werror("Failed to resolve the dtor function for the plugin from the library.");
    return false;
  }
  else
  {
    winfo("Resolved dtor for plugin.");
  }

  bool (*pluginDtorFunction)(Plugin::Ptr&);
  bool wasDestroyed = false;
  *(void **)(&pluginDtorFunction) = funcDtorHandle;
  assert (pluginDtorFunction);

  try
  {
    winfo("Invoking dtor function for plugin...");
    wasDestroyed = pluginDtorFunction(plugin);
    winfo("Invoked dtor function for plugin.");
  }
  catch (std::exception &e)
  {
    werror(string("Failed to kill plugin; e: %s") + e.what());
    return false;
  }

  if (libraryPtr->isLoaded())
  {
    const bool unloadedLibrary = libraryPtr->unload();
    auto errMsg = libraryPtr->errorMessage();
    wdebug("Was library for plugin " + name + " unloaded? " + std::to_string(unloadedLibrary));

    wdebug("Unloaded library for plugin " + name + " successfully? " + wbool2str(unloadedLibrary));
    if (errMsg.empty())
    {
      wwarn("Failed to unload library for plugin " + name + ": ");
    }
  }

  PluginPrivate::plugins.erase(pluginItr);
  return wasDestroyed;
}

Plugin::~Plugin()
{
  wdebug("Destroyed plugin " + name() + ".");
}
