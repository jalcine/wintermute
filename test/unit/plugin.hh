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

#include <cxxtest/TestSuite.h>
#include "libwintermutecore/plugin.hpp"
#include "test_suite.hpp"

using Wintermute::Plugin;

class PluginLibraryTestSuite : public CxxTest::TestSuite
{
public:
  // Test loading a binary from disk.
  void testUnloadsWhenOutOfScope()
  {
    Plugin::Ptr plugin = Plugin::load(SAMPLE_PLUGIN_PATH);
		TSM_ASSERT_EQUALS ( "Was plugin created?", !!plugin, true );

		if (plugin) {
			TSM_ASSERT ( "Was Plugin loaded successfully?", plugin->state() == Plugin::Loaded );
		}

    // We don't have to clean up; the pointer itself will be destroyed when we
    // leave this scope.
  }

  // TODO: Test loading a binary from the LD_LIBRARY_PATH.
  // TODO: Test loading a binary from our prepopulated paths.
  // TODO: Test unloading a binary.
  // TODO: Test resolving symbols from the binary.
};

class PluginTestSuite : public CxxTest::TestSuite
{
public:
  // TODO: Test starting the plugin.
  // TODO: Test stopping the plugin.
  // TODO: Test grabbing the plugin's instance.
};
