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
using Wintermute::LibraryHandle;

LibraryPrivate::HandlePtr LibraryPrivate::obtainHandle(const int& flags)
{
  auto rawHandle = dlopen(filePath.c_str(), flags);
  LibraryPrivate::HandlePtr handlePtr(static_cast<LibraryHandle*>(rawHandle));
  assert(handlePtr);
  wdebug("Was handle obtained? " + std::to_string((bool) handlePtr));

  return handlePtr;
}

bool LibraryPrivate::closeHandle(LibraryPrivate::HandlePtr& handlePtr)
{
  const int exitcode = dlclose(handlePtr.get());
  wdebug("Return code from dlclose() " + std::to_string(exitcode));
  return exitcode == 0;
}


LibraryPrivate::LibraryPrivate() : filePath(), handlePtr(nullptr)
{
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
  if (handlePtr)
  {
    wdebug("Unloading library " + filePath + "...");
    if (unload()) {
      wdebug("Unloaded library " + filePath + "; dereferencing pointer...");
      handlePtr = nullptr;
      wdebug("Pointer redeferened.");
    } else {
      werror("Failed to unload library.");
    }
  }

  assert(!handlePtr);
}


