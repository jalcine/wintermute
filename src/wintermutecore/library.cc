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

#include "logging.hpp"
#include "library.hpp"
#include "library.hh"

using Wintermute::Library;
using Wintermute::LibraryPrivate;

LibraryPrivate::LibraryPrivate() :
  handlePtr(0), filename(""), loadState(Library::LoadUndefined)
{
  handlePtr = new LibraryPrivate::HandlePtr;
}

LibraryPrivate::HandlePtr* LibraryPrivate::claimHandleForFilename(const string& filenameToLoad, string& errorMessage)
{
  wdebug("Claiming handle for library " + filenameToLoad + "...");
  LibraryPrivate::HandlePtr* handle = new LibraryPrivate::HandlePtr;

  const int openedLibrary = uv_dlopen(filenameToLoad.c_str(), handle);

  if (openedLibrary < 0)
  {
    const char* msg = uv_dlerror(handle);

    if (msg)
    {
      errorMessage = msg;
      werror("Failed to load library '" + filenameToLoad + "': " + errorMessage);
    }
    else
    {
      errorMessage = "Unknown error.";
      wwarn("Failed to load library due to an unknown error; possibly file missing.");
    }

    return nullptr;
  }

  winfo("Handle loaded successfully.");
  errorMessage = "";

  return handle;
}

bool LibraryPrivate::freeHandle(string& errorMessage)
{
  uv_dlclose(handlePtr);
  errorMessage = uv_dlerror(handlePtr);
  handlePtr = nullptr;
  return true;
}

LibraryPrivate::~LibraryPrivate()
{
}
