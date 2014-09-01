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
#include "plugin.hpp"
#include "plugin/library.hh"

using Wintermute::Plugin;
using Wintermute::LibraryPrivate;

LibraryPrivate::LibraryPrivate() : filePath(), handlePtr(nullptr)
{
}

LibraryPrivate::Handle LibraryPrivate::obtainHandle(const int& flags)
{
  LibraryPrivate::Handle handle = nullptr;

  // Use the flags with `dlopen` to open the handle.
  handle = dlopen(filePath.c_str(), flags);

  if (!handle)
  {
    werror("Failed to open the handle from disk: " + std::string(dlerror()));
    dlerror();
    return nullptr;
  }
  else
  {
    winfo("Opened handle from disk " + filePath + "!");
  }

  // OPTIONAL: Assert said handle.
#if WINTERMUTE_DEBUG
  assert(handle);
#endif

  // Return the handle.
  return handle;
}

bool LibraryPrivate::closeHandle(LibraryPrivate::Handle& handle)
{
  bool wasClosed = false;

  // Check that the provided handle isn't null.
  if (!handle || handle == nullptr)
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
    handlePtr = nullptr;
  }
  else
  {
    wdebug("Handle already cleared.");
  }
  wdebug("Deleted private data for library? " + std::to_string(!(bool) handlePtr));
}
