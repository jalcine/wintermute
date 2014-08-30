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

#include <dlfcn.h>
#include <sys/stat.h>
#include <cstdio>
#include <algorithm>
#include <functional>
#include <memory>
#include "plugin.hh"
#include "plugin.hpp"
#include "logging.hpp"
#include "globals.hpp"

using Wintermute::Plugin;
using Wintermute::PluginPrivate;

PluginPrivate::PluginMap PluginPrivate::plugins;

Plugin::Plugin(const string& name) : d_ptr(new PluginPrivate)
{
	W_PRV(Plugin);
	assert ( name.empty() == false );
	d->name = name;
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
	W_PRV ( Plugin );
	wtrace("Shutting down plugin " + name() + "...");
	const string pluginName = name();

	if ( state() == Plugin::Unloaded )
	{
		wdebug("Plugin " + pluginName + "already unloaded.");
		return Plugin::Unloaded;
	}
	else
	{
		wdebug("Stopping plugin " + pluginName + "...");
		this->shutdown();
		wdebug("Plugin " + pluginName + " stopped.");
	}

	return d->loadState;
}

Plugin::Ptr Plugin::load(const string& filepath)
{
	assert(!filepath.empty());
	wdebug("Attempting to load library " + filepath + " for plugin...");
	Plugin::Library::Ptr libraryPtr = Plugin::Library::find(filepath);
	wdebug("Was library " + filepath + " found on `ld`? " + (libraryPtr ? "YES" : "NO"));

	if (libraryPtr) {
		return load(libraryPtr);
	}

	return nullptr;
}

Plugin::Ptr Plugin::load(Library::Ptr& libraryPtr)
{
	Plugin::Ptr pluginPtr = { nullptr };
	if (!libraryPtr->isLoaded())
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
		pluginPtr = pluginCtorFunction();
		winfo("Invoked ctor function for plugin.");
	}
	catch (std::exception &e)
	{
		werror(string("Failed to load library for plugin; e: %s") + e.what());
		pluginPtr = nullptr;
		return pluginPtr;
	}

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

	if (pluginItr != PluginPrivate::plugins.end())
	{
		wdebug("Found plugin " + pluginItr->first + " to unload.");
		Plugin::Ptr plugin = pluginItr->second;
		auto unloadedLibrary = plugin->d_func()->libraryPtr->unload();

		if (unloadedLibrary)
		{
			wdebug("Unloaded library for plugin " + name + " successfully.");
			PluginPrivate::plugins.erase(pluginItr);
			return true;
		}
		else
		{
			wwarn("Failed to unload library for plugin " + name + ".");
		}
	}
	else
	{
		wdebug("Couldn't find plugin named " + name + ".");
	}

	return false;
}

Plugin::~Plugin()
{
	W_PRV ( Plugin );
	if (state() == Plugin::Loaded)
	{
		wwarn("Plugin still loaded yet being destroyed! Unloading from system...");
		Plugin::unload(name());
	}

	if (d->libraryPtr->isLoaded())
	{
		wwarn("Unloading plugin " + name() + "'s library...");
		d->libraryPtr->unload();
		d->libraryPtr = nullptr;
		wwarn("Unloaded library!");
	}
}

PluginPrivate::PluginPrivate() : name(), libraryPtr()
{
}

Plugin::Ptr PluginPrivate::attemptLoad()
{
	Plugin::Ptr pluginPtr;

	if (libraryPtr->load())
	{
		wtrace("Resolved the plugin's ctor function.");
		winfo("Created the plugin " + pluginPtr->name() + ".");
	}
	else
	{
		werror("The plugin's library failed to load.");
	}

	return pluginPtr;
}

PluginPrivate::~PluginPrivate()
{
}
