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

#include "test_suite.hpp"
#include "libwintermutecore/plugin.hpp"

using Wintermute::Plugin;

class PluginTestSuite : public CxxTest::TestSuite
{
public:
  void testStartAndStopPlugin(void)
  {
    setenv(WINTERMUTE_ENV_PLUGIN_PATH, string(getenv("PWD") + string("/test/fixtures")).c_str(), 1);
    Plugin::Ptr pluginPtr = W_CLAIM_SHARED_PTR(Plugin::load(SAMPLE_PLUGIN_NAME));

    TSM_ASSERT ( "Loads plugins with provided name.", pluginPtr );
    TSM_ASSERT_EQUALS ( "Reports loaded status.", pluginPtr->state(), Plugin::Loaded );
    TSM_ASSERT_EQUALS ( "Ensures that the plugin starts.", pluginPtr->start(), Plugin::Loaded );
    TSM_ASSERT_EQUALS ( "Ensures that the plugin stops.", pluginPtr->stop(), Plugin::Unloaded );
    TSM_ASSERT ( "Unloads the provided plugin.", Plugin::unload(SAMPLE_PLUGIN_NAME));
    TSM_ASSERT ( "Plugin isn't discoverable at run-time.", !Plugin::isLoaded(SAMPLE_PLUGIN_NAME));
    wdebug("Damn son, where'd you find this?");

    unsetenv(WINTERMUTE_ENV_PLUGIN_PATH);
  }

};
