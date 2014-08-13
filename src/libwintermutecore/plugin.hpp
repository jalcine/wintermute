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

#ifndef WINTERMUTE_PLUGIN_HPP
#define WINTERMUTE_PLUGIN_HPP

#include "globals.hpp"
#include <list>
#include <map>
#include <string>
#include <functional>

using std::map;
using std::list;
using std::string;
using std::function;

namespace Wintermute
{
class PluginPrivate;
class LibraryPrivate;
class Plugin
{
	W_DEFINE_PRIVATE(Plugin)

public:
	virtual ~Plugin();
	W_DECLARE_PTR_TYPE(Plugin)

		// Represents a list of Plugin pointers.
	typedef list<Plugin::Ptr> List;

	// Represents a sorted map of Plugin pointers.
	typedef map<string, Plugin::Ptr> Map;

	// Obtains the identifing map of this plugin.
	string name() const;

	virtual void start();
	virtual void stop();

	class Library
	{
	private:
		W_DEFINE_PRIVATE(Library)

	public:
		W_DECLARE_PTR_TYPE(Library)
		explicit Library(const string& libraryPath);
		virtual ~Library();

		// Represents a raw function pointer.
		typedef SharedPtr<void> FunctionHandlePtr;

		// Obtains the path to the library in question.
		string filePath() const;

		// Loads the library from disk.
		bool load();

		// Unloads the library from this application's memory.
		bool unload();

		bool isLoaded();

		// Attempts to obtain a function from the library.
		Library::FunctionHandlePtr resolveMethod(const string& methodName);
	};

	static Plugin::Ptr load(const string& filepath);
	static Plugin::Ptr load(Plugin::Library::Ptr& library);
	static bool unload(const string& name);
	static list<string> loadedPlugins();

protected:
	explicit Plugin(const string& name);

};
}

#define W_DEFINE_PLUGIN(Class) \
    Wintermute::Plugin::Ptr w_create_plugin(); \
    bool w_delete_plugin(Wintermute::Plugin::Ptr& pluginPtr); \
 
#define W_EXPOSE_PLUGIN(Class) \
	Wintermute::Plugin::Ptr w_create_plugin() { \
		return Wintermute::Plugin::Ptr(new Class); \
	} \
	bool w_delete_plugin(Wintermute::Plugin::Ptr& pluginPtr) { \
		pluginPtr.reset(); \
		return !!pluginPtr; \
	}

#endif
