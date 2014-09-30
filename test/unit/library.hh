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

#include <stdlib.h>
#include "test_suite.hpp"
#include "wintermutecore/logging.hpp"
#include "wintermutecore/library.hpp"

using Wintermute::Logging;
using Wintermute::Library;

Library::Ptr fetchWorkingLibrary()
{
  Library::Ptr libraryPtr = std::make_shared<Library>(SAMPLE_PLUGIN_PATH);
  TSM_ASSERT ( "Library pointer allocated with full file path.",
               libraryPtr );
  TSM_ASSERT_EQUALS ( "Library reports proper status.",
                      libraryPtr->loadedStatus(),
                      Library::LoadIsLoaded );
  TSM_ASSERT_EQUALS ( "Library reports provided file name.",
                      libraryPtr->filename(),
                      SAMPLE_PLUGIN_PATH );
  return libraryPtr;
}

class LibraryTestSuite : public CxxTest::TestSuite
{
public:
  void setUp()
  {
    //DISABLE_LOGGING;
  }

  void tearDown()
  {
    unsetenv(WINTERMUTE_ENV_PLUGIN_PATH);
  }

  void testLoadLibraryWithNoName()
  {
    Library::Ptr libraryWithNoName = std::make_shared<Library>("");
    TSM_ASSERT ( "Library pointer allocated.",
                 libraryWithNoName );
    TSM_ASSERT_EQUALS ( "Library not loaded.",
                        libraryWithNoName->loadedStatus(),
                        Library::LoadUndefined );
  }

  void testLoadLibraryWithFullFilePath()
  {
    Library::Ptr libraryWithFullFilePath = std::make_shared<Library>(SAMPLE_PLUGIN_PATH);
    TSM_ASSERT ( "Library pointer allocated with full file path.",
                 libraryWithFullFilePath );
    TSM_ASSERT_EQUALS ( "Library reports proper status.",
                        libraryWithFullFilePath->loadedStatus(),
                        Library::LoadIsLoaded );
    TSM_ASSERT_EQUALS ( "Library reports provided file name;.",
                        libraryWithFullFilePath->filename(),
                        SAMPLE_PLUGIN_PATH );
  }

  void testUnloadingLibrary()
  {
    Library::Ptr libraryPtr = fetchWorkingLibrary();
    TSM_ASSERT_EQUALS ( "Library unloaded successfully.",
                        libraryPtr->unload(),
                        Library::LoadNotLoaded );
    TSM_ASSERT_EQUALS ( "Library status = LoadNotLoaded",
                        libraryPtr->loadedStatus(),
                        Library::LoadNotLoaded );
    TSM_ASSERT ( "Wipe filename on unload.",
                 libraryPtr->filename().empty() );
  }

  void testDiscoverLibraryByName()
  {
    setenv(WINTERMUTE_ENV_PLUGIN_PATH, string(TEST_BASE_DIR "/fixtures").c_str(), 1);
    Library::Ptr libraryByNamePtr = Library::find(SAMPLE_PLUGIN_NAME);
    TSM_ASSERT ( "Library pointer allocated with full file path.",
                 libraryByNamePtr );
    TSM_ASSERT_EQUALS ( "Library reports proper status.",
                        libraryByNamePtr->loadedStatus(),
                        Library::LoadIsLoaded );
    TSM_ASSERT ( "Library reports provided file name.",
                 libraryByNamePtr->filename().find_last_of(SAMPLE_PLUGIN_FILE_NAME) != string::npos);
  }

  void testDiscoverLibraryByRelativeFileName()
  {
    setenv(WINTERMUTE_ENV_PLUGIN_PATH, TEST_BASE_DIR, 1);
    Library::Ptr libraryByRelativeFileNamePtr = Library::find("fixtures/" SAMPLE_PLUGIN_FILE_NAME);
    TSM_ASSERT ( "Library pointer allocated with full file path.",
                 libraryByRelativeFileNamePtr );
    TSM_ASSERT_EQUALS ( "Library reports proper status.",
                        libraryByRelativeFileNamePtr->loadedStatus(),
                        Library::LoadIsLoaded );
    TSM_ASSERT ( "Library reports provided file name.",
                 libraryByRelativeFileNamePtr->filename().find_last_of(SAMPLE_PLUGIN_FILE_NAME) != string::npos);
  }

  void testDiscoverLibraryByFullFilePath()
  {
    Library::Ptr libraryByFullFilePathPtr = Library::find(SAMPLE_PLUGIN_PATH);
    TSM_ASSERT ( "Library pointer allocated with name.",
                 libraryByFullFilePathPtr );
    TSM_ASSERT_EQUALS ( "Library reports proper status.",
                        libraryByFullFilePathPtr->loadedStatus(),
                        Library::LoadIsLoaded );
    TSM_ASSERT_EQUALS ( "Library reports provided file name.",
                        libraryByFullFilePathPtr->filename(),
                        SAMPLE_PLUGIN_PATH );
  }

  void testResolveNoFunctionFromLibrary()
  {
    Library::Ptr libraryPtr = fetchWorkingLibrary();
    Library::FunctionPtr okFunctionPtr = libraryPtr->resolveFunction("");
    TSM_ASSERT ( "Didn't resolved empty function name from library.",
                 !okFunctionPtr );
  }

  void testResolveFunctionFromLibrary()
  {
    Library::Ptr libraryPtr = fetchWorkingLibrary();
    Library::FunctionPtr okFunctionPtr = libraryPtr->resolveFunction("w_sample_test");
    TSM_ASSERT ( "Resolved function 'w_sample_test' from library.",
                 okFunctionPtr );
    int (*aFunction)(void);
    W_RESOLVE_FUNCTION(aFunction, okFunctionPtr);
    TSM_ASSERT_EQUALS ( "Function invoked with expected return value.",
                        aFunction(), 2014);
  }

};
