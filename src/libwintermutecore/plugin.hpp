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

namespace Wintermute
{
class PluginPrivate;
class Plugin
{
  W_DEFINE_PRIVATE(Plugin);

public:
  W_DECLARE_PTR_TYPE(Plugin);
  typedef std::list<Plugin::Ptr> List;
  typedef std::map<string, Plugin::Ptr> Map;
  typedef std::function<Plugin::Ptr(void)> EntryFunction;
  typedef void Handle;
  typedef SharedPtr<Handle> HandlePtr;

  string name() const;
  bool loaded() const;
  static Plugin::Ptr loadByName(const string& name);
  static Plugin::Ptr loadByFilePath(const string& filePath);
  static bool unload(const string& name);
  static Plugin::List plugins();

protected:
  explicit Plugin(const std::string& name);
};
}

Wintermute::Plugin::Ptr w_resolve_plugin_ptr(Wintermute::Plugin::HandlePtr handle);
Wintermute::Plugin::HandlePtr w_open_plugin_library(const string& fileName);

#define W_DECLARE_PLUGIN(Class) \
  Wintermute::Plugin::Ptr WINTERMUTE_ENTRY_FUNCTION() { \
    return Wintermute::Plugin::Ptr(new Class); \
  }

#define W_EXPOSE_PLUGIN(Class) w_noop();

#endif
