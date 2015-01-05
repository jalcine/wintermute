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

#ifndef WINTERMUTE_PRIVATE_PLUGIN_HPP
#define WINTERMUTE_PRIVATE_PLUGIN_HPP

#include <unordered_map>
#include <string>
#include "library.hpp"
#include "plugin.hpp"

using std::unordered_map;
using std::string;

namespace Wintermute
{
class PluginPrivate
{
public:
  typedef Plugin*(*CtorFunctionPtr)(void);
  typedef const char*(*VersionFunctionPtr)(void);
  typedef bool(*DtorFunctionPtr)(Plugin::Ptr&);
  typedef unordered_map<string, Plugin::Ptr> PluginMap;

  static PluginMap plugins;
  Library::Ptr library;
  string name;

  explicit PluginPrivate(const string& pluginName);
  virtual ~PluginPrivate();
  static bool registerPlugin(Plugin::Ptr& plugin);
  static bool unregisterPlugin(const string& pluginName);
};
}
#endif
