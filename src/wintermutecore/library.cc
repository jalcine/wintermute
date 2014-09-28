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
#include "logging.hpp"
#include "library.hpp"
#include "library.hh"

using Wintermute::Library;
using Wintermute::LibraryPrivate;

LibraryPrivate::LibraryPrivate() :
  handlePtr(nullptr), filename(""), loadState(Library::LoadUndefined)
{
}

LibraryPrivate::HandlePtr LibraryPrivate::claimHandleForFilename(const string& filename, string& errorMessage)
{
  wdebug("Claiming handle for library " + filename + "...");
  LibraryPrivate::HandlePtr handle = nullptr;

  dlerror();
  handle = dlopen(filename.c_str(), RTLD_NOW | RTLD_GLOBAL );
  const char* msg = dlerror();

  if (!handle && msg)
  {
    errorMessage = msg;
    werror("Failed to load library '" + filename + "': " + errorMessage);
    return nullptr;
  }

  winfo("Handle loaded successfully.");
  errorMessage = "";

  return handle;
}

bool LibraryPrivate::freeHandle(string& errorMessage)
{
  dlerror();
  const int wasntSuccessfully = dlclose(handlePtr);
  if (wasntSuccessfully) {
    errorMessage = dlerror();
  }
  return wasntSuccessfully == 0;
}

LibraryPrivate::~LibraryPrivate()
{
}
