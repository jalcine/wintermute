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

class PluginLibraryTestSuite : public CxxTest::TestSuite
{
public:
  // Test loading a binary from disk.
  void testLoad()
  {
    // TODO: Test loading a binary from the LD_LIBRARY_PATH.
    // TODO: Test loading a binary from our prepopulated paths.
    Plugin::Library::Ptr libraryPtr = std::make_shared<Plugin::Library>(SAMPLE_PLUGIN_PATH);
    TSM_ASSERT_EQUALS ( "Was library created?", (bool) libraryPtr, true );
    TSM_ASSERT_EQUALS ( "Was library loaded?", libraryPtr->load(), true );
  }

  void testUnload()
  {
    Plugin::Library::Ptr libraryPtr = std::make_shared<Plugin::Library>(SAMPLE_PLUGIN_PATH);
    TS_ASSERT ( libraryPtr->load() );
    TS_ASSERT ( libraryPtr->unload() );
  }

  // TODO: Test resolving symbols from the binary.
  void testResolveFunction()
  {
    Plugin::Library::Ptr libraryPtr = std::make_shared<Plugin::Library>(SAMPLE_PLUGIN_PATH);
    TS_ASSERT ( libraryPtr->load() );
    Plugin::Library::FunctionHandlePtr funcPtr = libraryPtr->resolveMethod(WINTERMUTE_PLUGIN_METHOD_CTOR_NAME);
    TS_ASSERT ( funcPtr );
    TS_ASSERT ( libraryPtr->unload() );
  }
};
