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
#include <system_error>
#include <boost/filesystem.hpp>
#include "plugin.hh"
#include "plugin.hpp"
#include "logging.hpp"

using Wintermute::Plugin;
using Wintermute::PluginPrivate;
namespace bfs = boost::filesystem;

// NOTE: RTLD_DEEPBIND only available in glibc 2.3.4.
Plugin::HandlePtr w_open_plugin_library(const string& filepath)
{
	Plugin::HandlePtr handlePtr(nullptr);
	const int ld_flags = RTLD_NOW | RTLD_GLOBAL | RTLD_DEEPBIND;
	dlerror();

	if (bfs::exists(filepath))
	{
		handlePtr.reset(dlopen(filepath.c_str(), ld_flags));
		const string potentialError(dlerror());

		if (!potentialError.empty())
		{
			werror("Failed to load the plugin at " + filepath + "; error message: " + potentialError);
			//throw new std::runtime_error("Failed to load library for plugin at " + filepath);
		}
	}
	else
	{
		werror("Could not find library at " + filepath + ".");
	}

	return handlePtr;
}

Plugin::Ptr w_resolve_plugin_ptr(Plugin::HandlePtr& handle)
{
	dlerror();
	assert(handle);

	Plugin::EntryFunction entryFunction(
	    (Plugin::EntryFunctionSignature*) dlsym(handle.get(), WINTERMUTE_ENTRY_FUNCTION_STR));

	assert(entryFunction);

	Plugin::Ptr pluginPtr(entryFunction());
	assert(pluginPtr);

	const char* libError = dlerror();
	if (libError)
	{
		const string fullMsg("Failed to load library: " + string(libError));
		werror(fullMsg);
		//throw new std::system_error(EADDRNOTAVAIL, std::system_category(), fullMsg);
		return Plugin::Ptr(nullptr);
	}

	assert(pluginPtr);
	assert(sizeof (pluginPtr.get()) == sizeof (Wintermute::Plugin*));

	// Check for a invalid pointer.
	if (!pluginPtr || sizeof (pluginPtr.get()) == sizeof (Wintermute::Plugin*))
	{
		pluginPtr.reset();
	}

	return pluginPtr;
}

bool w_close_plugin_library(Plugin::HandlePtr& handle)
{
	dlclose(handle.get());
	return false;
}

Plugin::Plugin(const string& name) : d_ptr(new PluginPrivate)
{
	W_PRV(Plugin);
	d->name = name;
}

string Plugin::name() const
{
	W_PRV(const Plugin);
	return d->name;
}

// TODO Pass in the handle to the pimpl of the Plugin for future releasing.
Plugin::Ptr Plugin::loadByFilePath(const string& filePath)
{
	assert(!filePath.empty());

	Plugin::HandlePtr handle = w_open_plugin_library(filePath);
	assert(handle);

	Plugin::Ptr plugin = w_resolve_plugin_ptr(handle);
	assert(plugin);

	return plugin;
}

Plugin::Ptr Plugin::loadByName(const string& libraryName)
{}

bool Plugin::unload(const string& name)
{
}

Plugin::~Plugin()
{
}

PluginPrivate::PluginPrivate()
{}

PluginPrivate::~PluginPrivate()
{}
