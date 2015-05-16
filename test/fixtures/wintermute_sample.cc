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

#define WINTERMUTE_TEST_SAMPLE 1
#include <wintermute-core/logging.hpp>
#include "test_suite.hpp"
#include "wintermute_sample.hh"

int w_sample_test()
{
  winfo("Invoking a function from beyond.");
  return 2014;
}

using Wintermute::Plugin;
using Wintermute::SamplePlugin;

SamplePlugin::SamplePlugin() : Plugin(SAMPLE_PLUGIN_NAME)
{
  wdebug("Created the sample plugin!");
}

SamplePlugin::~SamplePlugin()
{
  wdebug("Killed the sample plugin!");
}

bool SamplePlugin::startup()
{
  wdebug("Sample plugin started.");
  return true;
}

bool SamplePlugin::shutdown()
{
  wdebug("Sample plugin killed.");
  return true;
}

Plugin::PluginType SamplePlugin::type() const
{
  return Plugin::PluginTypeSupport;
}
