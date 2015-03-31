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
typedef std::unordered_map<string, LibraryPrivate::HandlePtr> LibraryHandleMap;

LibraryHandleMap LibraryPrivate::libraryHandles = LibraryHandleMap();

LibraryPrivate::LibraryPrivate() :
  handle(nullptr), filename(""), loadState(Library::LoadUndefined)
{
}

bool LibraryPrivate::claimHandle(const string& filenameToLoad)
{
  auto itr = LibraryPrivate::libraryHandles.find(filenameToLoad);
  if (itr != LibraryPrivate::libraryHandles.end())
  {
    wdebug("Fetching existing library handle.");
    handle = itr->second;
    return true;
  }

  LibraryPrivate::HandlePtr handlePtr = make_shared<LibraryPrivate::HandleType>();
  const int handleOpened = uv_dlopen(filenameToLoad.c_str(), handlePtr.get());

  if (handleOpened == 0)
  {
    auto pair = std::make_pair(filenameToLoad, handlePtr);
    LibraryPrivate::libraryHandles.insert(pair);
    handle = handlePtr;
    return true;
  }
  else
  {
    wdebug("Failed to load library: " + string(uv_dlerror(handlePtr.get())));
  }

  handle = nullptr;
  return false;
}

bool LibraryPrivate::freeHandle()
{
  if (handle)
  {
    uv_dlclose(handle.get());
    LibraryPrivate::libraryHandles.erase(filename);
  }

  return true;
}

LibraryPrivate::~LibraryPrivate()
{
}
