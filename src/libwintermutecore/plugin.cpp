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
#include <system_error>
#include "plugin.hh"
#include "plugin.hpp"

using Wintermute::Plugin;

Plugin::HandlePtr w_open_plugin_library(const string& filepath)
{
  Plugin::HandlePtr pluginHandlePtr(dlopen(filepath.c_str(), RTLD_LAZY));
  assert(pluginHandlePtr);
  return pluginHandlePtr;
}

Plugin::Ptr w_resolve_plugin_ptr(Plugin::HandlePtr handle)
{
  Plugin::Ptr pluginPtr;
  pluginPtr.reset(
    static_cast<Plugin::Ptr::element_type*>(
      dlsym(handle.get(), WINTERMUTE_ENTRY_FUNCTION)));
  assert(pluginPtr);

  const string libError = dlerror();
  assert(libError.empty());
  if (!libError.empty()) {
    throw new std::system_error(EADDRNOTAVAIL, std::system_category(), "Failed to load library.");
  }

  return pluginPtr;
}

bool w_close_plugin(Plugin::HandlePtr handle)
{
  return false;
}
