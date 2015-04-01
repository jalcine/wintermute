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

#include <list>
#include <wintermute-core/globals.hpp>
#include <wintermute-core/version.hpp>

using std::string;
using std::list;

namespace Wintermute
{
class PluginPrivate;

/* An interface to add functionality and form to Wintermute. */
class WINTERMUTE_EXPORT_PUBLIC Plugin
#ifndef DOXYGEN_SKIP
  : W_DEF_SHAREABLE(Plugin)
#endif
{

public:
  W_DEF_PRIVATE(Plugin)
  W_DECL_PTR_TYPE(Plugin)

  enum PluginType
  {
    PluginTypeUndefined = 0x000,
    PluginTypeSupport   = 0x100,
    PluginTypeService   = 0x200,
  };

  /* Default destructor. */
  ~Plugin();

  /* Obtains the name of the plugin. */
  string name() const;

  /* Obtains the version of this plugin. */
  Version version() const;

  /* Obtains the minimal version of Wintermute needed for this plugin. */
  Version systemVersion() const;

  /* Obtains the type of plugin that this is. */
  virtual PluginType type() const = 0;

  /* Looks up a plugin with the provided query on disk, loads and starts it. */
  static Ptr find(const string & pluginQuery);

  /* Finds the loaded plugin by query, stops it and unloads it. */
  static bool release(const string & pluginName);

  /* Queries the existence of this plugin in this instance of Wintermute. */
  static bool hasPlugin(const string & pluginName);

  static const list<string> all();

protected:
  /* Default constructor. */
  explicit Plugin(const string & name);

  /* VIRTUAL: Invoke when starting the plugin. */
  virtual bool startup() = 0;

  /* VIRTUAL: Invoke when stopping the plugin. */
  virtual bool shutdown() = 0;

};
}

#define WINTERMUTE_PLUGIN_CTOR_FUNCTION_NAME    "w_plugin_ctor"
#define WINTERMUTE_PLUGIN_DTOR_FUNCTION_NAME    "w_plugin_dtor"
#define WINTERMUTE_PLUGIN_VERSION_FUNCTION_NAME "w_plugin_version"

#define W_DECL_PLUGIN(PluginClass, WintermuteMinVersion) \
  extern "C" WINTERMUTE_EXPORT_PUBLIC PluginClass* w_plugin_ctor() { \
    return new PluginClass();\
  } \
  extern "C" WINTERMUTE_EXPORT_PUBLIC bool w_plugin_dtor(Wintermute::Plugin::Ptr& pluginPtr) { \
    return (bool) pluginPtr; \
  } \
  extern "C" WINTERMUTE_EXPORT_PUBLIC const char* w_plugin_version() { \
    return #WintermuteMinVersion; \
  }

#endif

