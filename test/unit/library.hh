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

class LibraryTestSuite : public CxxTest::TestSuite
{
public:
  Library::Ptr libraryPtr;

  void tearDown()
  {
    unsetenv(WINTERMUTE_ENV_PLUGIN_PATH);
    if (!libraryPtr)
    {
      libraryPtr.reset();
    }
  }

  void setUp()
  {
    if (libraryPtr && libraryPtr->loadedStatus() == Library::LoadIsLoaded)
    {
      TS_ASSERT ( libraryPtr->unload() );
    }

    if (!libraryPtr)
    {
      libraryPtr.reset();
    }
  }

  Library::Ptr fetchWorkingLibrary(const string& path = SAMPLE_PLUGIN_PATH)
  {
    libraryPtr = std::make_shared<Library>(SAMPLE_PLUGIN_PATH);

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
    Library::Ptr libraryWithFullFilePath = fetchWorkingLibrary(SAMPLE_PLUGIN_PATH);

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

    libraryPtr = fetchWorkingLibrary(SAMPLE_PLUGIN_NAME);
    TSM_ASSERT ( "Library pointer allocated with full file path.",
      libraryPtr );

    TSM_ASSERT_EQUALS ( "Library reports proper status.",
      libraryPtr->loadedStatus(),
      Library::LoadIsLoaded );

    TSM_ASSERT ( "Library reports provided file name.",
      libraryPtr->filename().find_last_of(SAMPLE_PLUGIN_FILE_NAME) != string::npos);
  }

  void testDiscoverLibraryByRelativeFileName()
  {
    setenv(WINTERMUTE_ENV_PLUGIN_PATH, TEST_BASE_DIR, 1);

    libraryPtr = fetchWorkingLibrary("fixtures/" SAMPLE_PLUGIN_FILE_NAME);

    TSM_ASSERT ( "Library pointer allocated with full file path.",
      libraryPtr );

    TSM_ASSERT_EQUALS ( "Library reports proper status.",
      libraryPtr->loadedStatus(),
      Library::LoadIsLoaded );

    TSM_ASSERT ( "Library reports provided file name.",
      libraryPtr->filename().find_last_of(SAMPLE_PLUGIN_FILE_NAME) != string::npos);
  }

  void testDiscoverLibraryByFullFilePath()
  {
    libraryPtr = fetchWorkingLibrary(SAMPLE_PLUGIN_PATH);

    TSM_ASSERT ( "Library pointer allocated with name.",
      libraryPtr );

    TSM_ASSERT_EQUALS ( "Library reports proper status.",
      libraryPtr->loadedStatus(),
      Library::LoadIsLoaded );

    TSM_ASSERT_EQUALS ( "Library reports provided file name.",
      libraryPtr->filename(),
      SAMPLE_PLUGIN_PATH );
  }

  void testResolveNoFunctionFromLibrary()
  {
    libraryPtr = fetchWorkingLibrary();
    Library::FunctionPtr okFunctionPtr = libraryPtr->resolveFunction("");
    TSM_ASSERT ( "Didn't resolved empty function name from library.",
      !okFunctionPtr );
  }

  void testResolveFunctionFromLibrary()
  {
    libraryPtr = fetchWorkingLibrary();
    Library::FunctionPtr okFunctionPtr = libraryPtr->resolveFunction("w_sample_test");

    TSM_ASSERT ( "Resolved function 'w_sample_test' from library.",
      okFunctionPtr );

    int (*aFunction)(void);
    W_RESOLVE_FUNCTION(aFunction, okFunctionPtr);

    TSM_ASSERT_EQUALS ( "Function invoked with expected return value.",
      aFunction(), 2014);
  }

};

