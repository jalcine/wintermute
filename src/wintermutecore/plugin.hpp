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

#include <globals.hpp>

namespace Wintermute
{

class Plugin;
class PluginPrivate;

class Plugin : W_DEF_SHAREABLE(Plugin)
{
  W_DEFINE_PRIVATE(Plugin);
public:
  /* Defines the state of a plugin when it relates to loading. */
  enum LoadState
  {
    LoadUnknown = 0x000,      /* Defines the unknown load state. */
    Loaded      = 0x100,      /* Defines the successfully loaded state of a plugin. */
    LoadFailure = 0x200,      /* Defines the failure of a plugin to load. */
  };

  /* Defines the kinds of failures a plugin can have when it fails to load. */
  enum LoadFailure
  {
    LoadFailureNone   = 0x000, /* Defines a state at which a plugin had no failures loaded. */
    LoadFailureFile   = 0x010, /* Defines a state where a I/O operation failed. */
    LoadFailureSymbol = 0x020, /* Defines a state where symbol resolution failed. */
    LoadFailureABI    = 0x030, /* Defiens a state where ABI incompabilities occured. */
  };

  /* Default destructor for plugins. */
  virtual ~Plugin();

  /* Attempts to load a plugin.
   * @param pluginName The name of the plugin to load; like 'wintermutecore'.
   * @reutun LoadFailure A value from the load failure enum.
   */
  static LoadFailure load(const string & pluginName);

  /* Attempts to unload a plugin.
   * @param pluginName The name of the plugin to unload; like 'wintermutecore'.
   * @reutun LoadFailure A value from the load failure enum.
   */
  static LoadFailure unload(const string & pluginName);

  /* The state of this plugin.
   * @return A value from LoadState that represents the load state.
   */
  LoadState state() const;
  // start()
  // stop()
  // static unload()
  // static isLoaded()
};

}

#endif
