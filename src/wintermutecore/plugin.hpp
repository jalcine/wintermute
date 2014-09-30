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

#include <wintermutecore/globals.hpp>

#define WINTERMUTE_PLUGIN_CTOR_FUNCTION_NAME "w_plugin_ctor"
#define WINTERMUTE_PLUGIN_DTOR_FUNCTION_NAME "w_plugin_dtor"

using std::string;

namespace Wintermute
{
class PluginPrivate;
/* An interface to add function to Wintermute. */
class Plugin : W_DEF_SHAREABLE(Plugin)
{
public:
  W_DEF_PRIVATE(Plugin);
  W_DECL_PTR_TYPE(Plugin);

  ~Plugin();
  string name() const;
  static Ptr find(const string & pluginQuery);
  static bool release(const string & pluginName);
  static bool hasPlugin(const string & pluginName);
  bool start();
  bool stop();

protected:
  explicit Plugin(const string & name);
  virtual bool startup() = 0;
  virtual bool shutdown() = 0;

};
}

#define W_DECL_PLUGIN(PluginClass) \
  extern "C" WINTERMUTE_EXPORT PluginClass* w_plugin_ctor() { \
    return new PluginClass();\
  } \
  extern "C" WINTERMUTE_EXPORT bool w_plugin_dtor(Wintermute::Plugin::Ptr& pluginPtr) { \
    return (bool) pluginPtr; \
  }

#endif
