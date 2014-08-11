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
#include "plugin.hh"
#include "plugin.hpp"
#include "logging.hpp"

using Wintermute::Plugin;
using Wintermute::PluginPrivate;

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

Plugin::Ptr Plugin::load(Library::Ptr& library)
{
	assert(library);

	Plugin::Ptr pluginPtr;

	if (library->load()){
		wtrace("Library successfully loaded.");
		//auto pluginCtorFunc = library->resolveMethod<Plugin* (void)>("w_create_plugin");
	} else {
		werror("Failed to load plugin library.");
	}

	return pluginPtr;
}

Plugin::Ptr Plugin::load(const string& filepath)
{
	assert(!filepath.empty());
	Library::Ptr libraryPtr(new Library(filepath));
	return load(libraryPtr);
}

void Plugin::start() 
{
}

void Plugin::stop()
{}

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
