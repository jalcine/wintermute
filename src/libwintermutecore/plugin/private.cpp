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

#include "plugin.hpp"
#include "logging.hpp"
#include "plugin.hh"

using Wintermute::Plugin;
using Wintermute::PluginPrivate;
PluginPrivate::PluginMap PluginPrivate::plugins;

PluginPrivate::PluginPrivate() : name(), libraryPtr()
{
}

PluginPrivate::~PluginPrivate()
{
  if (libraryPtr && libraryPtr->isLoaded())
  {
    wwarn("Unloading plugin " + name + "'s library...");
    if (libraryPtr->unload())
    {
      wwarn("Unloaded library for plugin " + name + "!");
    }
    else
    {
      werror("Failed to unload library for " + name + ": " + libraryPtr->errorMessage());
    }
  }
  else
  {
    winfo("No library loaded for this plugin; no need to unload anything.");
  }
}
