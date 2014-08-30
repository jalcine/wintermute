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

#include "plugin_library.hh"
#include "plugin.hpp"
#include "logging.hpp"
#include <stdexcept>
#include <dlfcn.h>

using Wintermute::Plugin;
using Wintermute::LibraryPrivate;
using Wintermute::LibraryHandle;

#define DLOPEN_FLAGS RTLD_NOW | RTLD_GLOBAL

Plugin::Library::Library(const string& filePath) : d_ptr(new LibraryPrivate)
{
  W_PRV(Library);
  d->filePath = filePath;
}

bool Plugin::Library::load()
{
  W_PRV(Library);
  assert(!d->handlePtr);
  assert(!d->filePath.empty());
  dlerror(); // empty out the error queue

  winfo("Opening library " + d->filePath + " ...");
  d->handlePtr = d->obtainHandle(DLOPEN_FLAGS);

  const string theMessage = errorMessage();
  assert(theMessage.empty());

  if (!theMessage.empty())
  {
    werror("Error loading the library: " + theMessage);
    d->handlePtr = nullptr;
  }

  if (d->handlePtr)
  {
    winfo("Opened library " + d->filePath + ".");
  }
  else
  {
    wwarn("Failed to open library " + d->filePath);
  }

  return (bool) d->handlePtr;
}

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

bool Plugin::Library::unload()
{
  W_PRV(Library);
  dlerror();

  if (isLoaded())
  {
    const bool wasClosed = d->unload();
    const string error = errorMessage();

    wdebug("Was the library internally closed? " + std::to_string(wasClosed));

    if (!error.empty())
    {
      werror("Error unloading the library: " + error);
      return false;
    }

    return wasClosed;
  }

  return true;
}

bool Plugin::Library::isLoaded() const
{
  W_PRV(const Library);
  return (bool) d->handlePtr;
}

Plugin::Library::FunctionHandlePtr Plugin::Library::resolveMethod(const string& methodName) const
{
  W_PRV(const Library);
  Library::FunctionHandlePtr functionHandlePtr(dlsym(d->handlePtr.get(), methodName.c_str()));

  if (!functionHandlePtr)
  {
    functionHandlePtr = nullptr;
    throw new std::domain_error("Failed to load library's handle: " + errorMessage());
  }

  return functionHandlePtr;
}

string Plugin::Library::errorMessage() const
{
  const char* msg = dlerror();
  return msg == nullptr ? "" : std::string(msg);
}

Plugin::Library::Ptr Plugin::Library::find(const string& filepath)
{
  wdebug("Searching for library at " + filepath + "...");
  Plugin::Library::Ptr libraryPtr(std::make_shared<Plugin::Library>(filepath));
  assert(libraryPtr->load());

  wdebug("Was the library loaded? " + std::to_string(libraryPtr->isLoaded()));
  if (!libraryPtr->isLoaded())
  {
    libraryPtr = nullptr;
  }

  return libraryPtr;
}

Plugin::Library::~Library()
{
  // d_ptr handles the unloading already.
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

LibraryHandle::LibraryHandle(Handle* a_ptr) : ptr(nullptr)
{
  if (a_ptr)
  {
    ptr = a_ptr;
  }
}

LibraryHandle::~LibraryHandle()
{
  return;
  int exitcode;
  winfo("Attempting to free the library handle...");
  try
  {
    wdebug("We have a library handle? " + std::to_string(ptr != nullptr));
    if (ptr != nullptr)
    {
      wdebug("Free up the library handle..");
      exitcode = dlclose(ptr);
    }
    ptr = nullptr;
  }
  catch ( std::exception& e )
  {
    winfo("Failed to free the library handle: " + std::string(e.what()));
  }

  winfo("Freed the library handle? " + std::to_string(exitcode));
}
