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
#include "plugin.hh"
#include "plugin.hpp"
#include "logging.hpp"

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
		winfo("Plugin " + name() + "already loaded; attempt at double-load?");
		return Plugin::Loaded;
	}

	try
	{
		d->loadState = this->startup() ? Plugin::Loaded : Plugin::LoadingFailed;
	}
	catch ( std::exception& e )
	{
		d->loadState = Plugin::LoadingFailed;
		d->loadFailure = Plugin::FailureABIException;
		wwarn("Failed to load plugin '" + name() + "' due to " + e.what() + ".");
	}

	return d->loadState;
}

Plugin::Ptr Plugin::load(Library::Ptr& libraryPtr)
{
	assert(libraryPtr);
	Plugin::Ptr pluginPtr(nullptr);
	if (!libraryPtr->isLoaded())
	{
		const bool wasLibraryLoaded = libraryPtr->load();

		if (!wasLibraryLoaded)
		{
			werror("Failed to load library! " + libraryPtr->lastErrorMessage());
			return pluginPtr;
		}
	}

	auto funcCtorHandle = libraryPtr->resolveMethod(WINTERMUTE_PLUGIN_METHOD_CTOR_NAME);

	if (!funcCtorHandle)
	{
		werror("Failed to resolve the ctor function for the plugin from the library.");
		return pluginPtr;
	}

	PluginPrivate::CtorSignaturePtr funcCtor(static_cast<PluginPrivate::CtorSignature*>(funcCtorHandle.get()));
	assert (funcCtor);

	try
	{
		pluginPtr = (*funcCtor)();
	}
	catch (std::exception &e)
	{
		werror(string("Failed to load library for plugin; e: %s") + e.what());

		return pluginPtr;
	}

	pluginPtr->d_ptr->loadState = Plugin::Loaded;
	pluginPtr->d_ptr->loadFailure = Plugin::FailureNone;
	winfo("Plugin successfully loaded!");

	return pluginPtr;
}

list<string> Plugin::loadedPlugins()
{
	list<string> pluginList;

	std::for_each(
	    PluginPrivate::plugins.cbegin(),
	    PluginPrivate::plugins.cend(),
	    [&](const Plugin::Map::value_type & pair)
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

Plugin::Ptr Plugin::load(const string& filepath)
{
	Plugin::Library::Ptr libraryPtr = Plugin::Library::find(filepath);
	return load(libraryPtr);
}

bool Plugin::unload(const string& name)
{
	return false;
}

Plugin::~Plugin()
{
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
		winfo("Created the plugin " + pluginPtr->name() + ".")
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
