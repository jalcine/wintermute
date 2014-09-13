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

#include <dlfcn.h>
#include <libgen.h>
#include <stdlib.h>
#include "logging.hpp"
#include "plugin.hpp"
#include "plugin/library.hh"

using Wintermute::Plugin;
using Wintermute::LibraryPrivate;

list<string> collectFilePaths(const string& filePath)
{
  // TODO: Build up a list of file paths to check.
  //  - $WINTERMUTE_PLUGIN_PATH could be a comma-separated list.
  //  - dlopen() already tries to use a few tactics to look up file paths.
  const string baseFilePath = { basename(const_cast<char*>(filePath.c_str())) };
  const char* envWintermutePluginPath = getenv(WINTERMUTE_ENV_PLUGIN_PATH);
  const string ourPathFromEnv = (envWintermutePluginPath) ? envWintermutePluginPath : "";

  list<string> prefixes;
  // Compile a list of paths where this thing can be at.
  list<string> filePaths =
  {
    filePath,
    "../" + baseFilePath,
    "../" + filePath,
    baseFilePath + ".so",
    filePath + ".so",
    "lib" + baseFilePath + ".so",
    "libwintermute" + baseFilePath + ".so",
  };
  list<string> originalPaths = filePaths;

  const bool hasAList = ourPathFromEnv.find(";") != string::npos;

  wdebug("Environment provided 'WINTERMUTE_PLUGIN_PATH' " + ourPathFromEnv);

  if (!hasAList)
  {
    prefixes.push_back(ourPathFromEnv);
    wdebug("Only one path provided.");
  }
  else
  {
    wdebug("Number of prefixes to use: " + std::to_string(prefixes.size()));
  }

  for (string path : originalPaths)
  {
    for (string prefix : prefixes)
    {
      const string thePrefixedPath = prefix + "/" + path;
      filePaths.insert(filePaths.end(), thePrefixedPath);
    }
  }

  return filePaths;
}

LibraryPrivate::LibraryPrivate() : filePath(), handlePtr(nullptr)
{
}

LibraryPrivate::Handle LibraryPrivate::obtainHandle(const int& flags)
{
  LibraryPrivate::Handle handle = nullptr;
  list<string> filePaths = collectFilePaths(filePath);

  for ( string aPath : filePaths )
  {
    wdebug("Attempting to open library handle from file '" + aPath + "'...");
    handle = dlopen(aPath.c_str(), flags);

    if (!handle)
    {
      werror("Failed to open the handle from disk: " + std::string(dlerror()));
      dlerror();
      continue;
    }
    else
    {
      filePath = aPath;
      winfo("Opened handle from disk " + aPath + "!");
      break;
    }
  }

  // Return the handle.
  return handle;
}

bool LibraryPrivate::closeHandle(LibraryPrivate::Handle& handle)
{
  bool wasClosed = false;

  // Check that the provided handle isn't null.
  if (!handle)
  {
    wwarn("Provided a null handle to work with.");
    return true;
  }

  // Close the handle provided using `dlclose`.
  // Determine the code returned by `dlclose`.
  const int dlCloseCode = dlclose(handle);
  wasClosed = (dlCloseCode == 0);
  wdebug("Was the handle closed successfully? " + std::to_string(wasClosed) + \
         " with code " + std::to_string(dlCloseCode));

  if (wasClosed)
  {
    handle = nullptr;
  }

  // Return true if code == 0, false otherwise.
  return wasClosed;
}

bool LibraryPrivate::unload()
{
  if (!handlePtr)
  {
    wdebug("Library already unloaded.");
    return true;
  }

  wdebug("Releasing handle for library...");
  const bool wasReleased = closeHandle(handlePtr);
  wdebug("Was the handle closed? " + std::to_string(wasReleased));

  return wasReleased;
}

LibraryPrivate::~LibraryPrivate()
{
  wdebug("Deleting private data for library...");
  if (handlePtr)
  {
    wdebug("Clearing handle to library...");
    closeHandle(handlePtr);
  }
  else
  {
    wdebug("Handle already cleared.");
  }

  wdebug("Deleted private data for library? " + std::to_string(!!handlePtr));
}
