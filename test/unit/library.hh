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
#include "wintermute-core/logging.hpp"
#include "wintermute-core/library.hpp"

using Wintermute::Logging;
using Wintermute::Library;

class LibraryTestSuite : public CxxTest::TestSuite
{
public:
  Library::Ptr libraryPtr;

  void tearDown()
  {
    unsetenv(WINTERMUTE_ENV_PLUGIN_PATH);
    libraryPtr = nullptr;
  }

  void setUp()
  {
    if (libraryPtr && libraryPtr->loadedStatus() == Library::LoadIsLoaded)
    {
      TS_ASSERT ( libraryPtr->unload() );
    }

    libraryPtr = nullptr;
  }

  Library::Ptr fetchWorkingLibrary(const string& thePath = SAMPLE_PLUGIN_PATH)
  {
    libraryPtr = std::make_shared<Library>(thePath);

    TSM_ASSERT (
      "Library pointer allocated with " + thePath,
      libraryPtr
    );

    TSM_ASSERT_EQUALS (
      "Library loads successfully.",
      libraryPtr->loadedStatus(),
      Library::LoadIsLoaded );

    TSM_ASSERT_EQUALS ( "Library reports provided file name.",
      libraryPtr->filename(),
      thePath );

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
    Library::Ptr libraryWithFullFilePath =
      std::make_shared<Library>(SAMPLE_PLUGIN_PATH);

    TSM_ASSERT ( "Library pointer allocated with full file path.",
      libraryWithFullFilePath );

    TSM_ASSERT_EQUALS ( "Library reports proper status.",
      libraryWithFullFilePath->loadedStatus(),
      Library::LoadIsLoaded );

    TSM_ASSERT_EQUALS ( "Library reports provided file name.",
      libraryWithFullFilePath->filename(),
      SAMPLE_PLUGIN_PATH );
  }

  void testUnloadingLibrary()
  {
    Library::Ptr aLibraryPtr = fetchWorkingLibrary();

    TSM_ASSERT_EQUALS ( "Library unloaded successfully.",
      aLibraryPtr->unload(),
      Library::LoadNotLoaded );

    TSM_ASSERT_EQUALS ( "Library status = LoadNotLoaded",
      aLibraryPtr->loadedStatus(),
      Library::LoadNotLoaded );

    TSM_ASSERT ( "Wipe filename on unload.",
      aLibraryPtr->filename().empty() );
  }

  void testDiscoverLibraryByName()
  {
    setenv(WINTERMUTE_ENV_PLUGIN_PATH,
      string(TEST_BASE_DIR "/../lib").c_str(), 1);

    libraryPtr = Library::find(SAMPLE_PLUGIN_SIMPLE_NAME);

    TSM_ASSERT (
      "Library pointer allocated.",
      libraryPtr
    );

    TSM_ASSERT_EQUALS (
      "Library reports proper status.",
      libraryPtr->loadedStatus(),
      Library::LoadIsLoaded
    );

    TSM_ASSERT_DIFFERS (
      "Library reports provided file name.",
      libraryPtr->filename().find_last_of(SAMPLE_PLUGIN_FILE_NAME),
      string::npos
    );
  }

  void testDiscoverLibraryByRelativeFileName()
  {
    setenv(WINTERMUTE_ENV_PLUGIN_PATH, TEST_BASE_DIR, 1);

    libraryPtr = Library::find("lib/" SAMPLE_PLUGIN_FILE_NAME);

    TSM_ASSERT ( "Library pointer allocated with full file path.",
      libraryPtr );

    TSM_ASSERT_EQUALS ( "Library reports proper status.",
      libraryPtr->loadedStatus(),
      Library::LoadIsLoaded );

    TSM_ASSERT_DIFFERS ( "Library reports provided file name.",
      libraryPtr->filename().find_last_of(SAMPLE_PLUGIN_FILE_NAME),
      string::npos);
  }

  void testDiscoverLibraryByFullFilePath()
  {
    libraryPtr = Library::find(SAMPLE_PLUGIN_PATH);

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

    int (*aFunction)(void) = 0;
    W_RESOLVE_FUNCTION(aFunction, okFunctionPtr);

    TSM_ASSERT ( "Valid function pointer obtained.",
      aFunction);

    TSM_ASSERT_EQUALS ( "Function invoked with expected return value.",
      aFunction(), 2014);
  }

};

