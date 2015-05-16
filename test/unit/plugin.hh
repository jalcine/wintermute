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
#include <wintermute-core/logging.hpp>
#include <wintermute-core/plugin.hpp>

using Wintermute::Logging;
using Wintermute::Plugin;

Plugin::Ptr fetchWorkingPlugin()
{
  setenv(WINTERMUTE_ENV_PLUGIN_PATH, string(TEST_LIB_DIR).c_str(), 1);
  Plugin::Ptr pluginPtr(Plugin::find(SAMPLE_PLUGIN_SIMPLE_NAME));
  unsetenv(WINTERMUTE_ENV_PLUGIN_PATH);
  TSM_ASSERT ( "Plugin allocated.", pluginPtr );
  return pluginPtr;
}

class PluginTestSuite : public CxxTest::TestSuite
{
public:
  Plugin::Ptr pluginPtr = nullptr;

  void setUp()
  {
    pluginPtr = nullptr;
  }

  void tearDown()
  {
    if (pluginPtr)
    {
      TSM_ASSERT ( "Plugin released from library.",
        Plugin::release(pluginPtr->name()) );
      pluginPtr = nullptr;
    }
  }

  void testFindLoadedPlugins()
  {

    TSM_ASSERT ( "Plugin 'sample' not found in listing.",
      Plugin::hasPlugin(SAMPLE_PLUGIN_NAME) == false);

    {
      setenv(WINTERMUTE_ENV_PLUGIN_PATH, string(TEST_LIB_DIR).c_str(), 1);
      pluginPtr = Plugin::find(SAMPLE_PLUGIN_SIMPLE_NAME);
      unsetenv(WINTERMUTE_ENV_PLUGIN_PATH);
    }

    TSM_ASSERT ( "Plugin '" SAMPLE_PLUGIN_NAME "' found in listing.",
      Plugin::hasPlugin(SAMPLE_PLUGIN_NAME) == true);
  }

  void testDontFindMissingPlugins()
  {
    TSM_ASSERT ( "Plugin 'foobar' not found in listing.",
      Plugin::hasPlugin("foobar") == false);
  }

  void testRetainsNameOfPlugin()
  {
    pluginPtr = fetchWorkingPlugin();

    TSM_ASSERT_EQUALS ( "Has the correct name",
      pluginPtr->name(), SAMPLE_PLUGIN_NAME);
  }

  void testExposesTypeOfPlugin()
  {
    pluginPtr = fetchWorkingPlugin();

    TSM_ASSERT ( "Isn't an undefined value.",
      pluginPtr->type() != Plugin::PluginTypeUndefined );
  }
};
