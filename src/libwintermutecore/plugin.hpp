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
class Plugin
{
	W_DEFINE_PRIVATE(Plugin);

public:
	virtual ~Plugin();
	W_DECLARE_PTR_TYPE(Plugin);

	// Represents a list of Plugin pointers.
	typedef list<Plugin::Ptr> List;

	// Represents a sorted map of Plugin pointers.
	typedef map<string, Plugin::Ptr> Map;

	// The signature of the function in question.
	typedef Plugin::Ptr (EntryFunctionSignature)(void);

	// Represents the signature of the entry method expected in plugin libraries.
	typedef function<EntryFunctionSignature> EntryFunction;

	// A typedef to hold the handle used to manipulate the library.
	typedef SharedPtr<void> HandlePtr;

	string name() const;

	static Plugin::Ptr loadByName(const string& libraryName);
	static Plugin::Ptr loadByFilePath(const string& filepath);
	static bool unload(const string& name);
	static Plugin::List plugins();

protected:
	explicit Plugin(const string& name);
};
}

Wintermute::Plugin::Ptr w_resolve_plugin_ptr(Wintermute::Plugin::HandlePtr& handle);
Wintermute::Plugin::HandlePtr w_open_plugin_library(const string& fileName);
bool w_close_plugin_library(Wintermute::Plugin::HandlePtr& handle);

#define W_DECLARE_PLUGIN(Class) \
	extern "C" Wintermute::Plugin::Ptr WINTERMUTE_ENTRY_FUNCTION(); \

#define W_EXPOSE_PLUGIN(Class) \
  extern "C" Wintermute::Plugin::Ptr WINTERMUTE_ENTRY_FUNCTION() \
  { \
    return Wintermute::Plugin::Ptr(new Class); \
  }

#endif
