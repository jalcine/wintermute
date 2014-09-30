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
#include <wintermutecore/logging.hpp>
#include <wintermutecore/plugin.hpp>

using Wintermute::Logging;
using Wintermute::Plugin;

Plugin::Ptr fetchWorkingPlugin()
{
  setenv(WINTERMUTE_ENV_PLUGIN_PATH, string(TEST_BASE_DIR "/fixtures").c_str(), 1);
  Plugin::Ptr pluginPtr(Plugin::find(SAMPLE_PLUGIN_NAME));
  unsetenv(WINTERMUTE_ENV_PLUGIN_PATH);
  TSM_ASSERT ( "Plugin allocated.", pluginPtr );
  return pluginPtr;
}

class PluginTestSuite : public CxxTest::TestSuite
{
public:
  void setUp()
  {
    DISABLE_LOGGING;
  }

  void testFindLoadedPlugins()
  {

    TSM_ASSERT ( "Plugin 'sample' not found in listing.",
                 Plugin::hasPlugin(SAMPLE_PLUGIN_NAME) == false);

    {
      setenv(WINTERMUTE_ENV_PLUGIN_PATH, string(TEST_BASE_DIR "/fixtures").c_str(), 1);
      Plugin::Ptr pluginPtr(Plugin::find(SAMPLE_PLUGIN_NAME));
      unsetenv(WINTERMUTE_ENV_PLUGIN_PATH);
    }

    TSM_ASSERT ( "Plugin '" SAMPLE_PLUGIN_NAME "' found in listing.",
                 Plugin::hasPlugin(SAMPLE_PLUGIN_NAME) == true);

    Plugin::release(SAMPLE_PLUGIN_NAME);
  }

  void testDontFindMissingPlugins()
  {
    TSM_ASSERT ( "Plugin 'foobar' not found in listing.",
                 Plugin::hasPlugin("foobar") == false);
  }

  void testRetainsNameOfPlugin()
  {
    Plugin::Ptr pluginPtr(fetchWorkingPlugin());
    TS_ASSERT ( pluginPtr );
    TSM_ASSERT_EQUALS ( "Has the correct name", pluginPtr->name(), SAMPLE_PLUGIN_NAME);
  }

  void testExposesTypeOfPlugin()
  {
    Plugin::Ptr pluginPtr(fetchWorkingPlugin());
    TS_ASSERT ( pluginPtr );
    TSM_ASSERT ( "Isn't an undefined value.", pluginPtr->type() != Plugin::PluginTypeUndefined );
  }
};
