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

#include "module.hh"
#include "module.hpp"
#include "message.hpp"
#include "logging.hpp"

using Wintermute::Module;
using Wintermute::Call;
using Wintermute::ModulePrivate;
using std::to_string;
using std::dynamic_pointer_cast;

Module::Module(const Module::Designation& aDesignation) :
  enable_shared_from_this(),
  d_ptr(std::make_shared<ModulePrivate>(aDesignation))
{
  wtrace("Module " + static_cast<string>(aDesignation) + " started.");
}

Module::Designation Module::designation() const
{
  W_PRV(Module);
  return d->designation;
}

bool Module::enable()
{
  if (isEnabled())
  {
    winfo("Module " + static_cast<string>(designation()) + " already exists in the pool.");
    return true;
  }

  wdebug("Module " + static_cast<string>(designation()) + " doesn't exist in the pool, adding..");
  Module::Ptr modulePtr = shared_from_this();
  const bool moduleAdded = Module::Pool::instance()->add(modulePtr);
  wdebug("Was module " + static_cast<string>(designation()) + " added into pool? " + to_string((int) moduleAdded));

  assert(moduleAdded);

  return moduleAdded;
}

bool Module::disable()
{
  if (!isEnabled())
  {
    winfo("Module " + static_cast<string>(designation()) + " already removed from the pool.");
    return true;
  }

  wdebug("Module " + static_cast<string>(designation()) + " exists in the pool, removing..");
  Module::Designation theDesignation = designation();
  const bool wasRemoved = Module::Pool::instance()->remove(theDesignation);
  assert(wasRemoved);
  // TODO: Clean this up.
  wdebug ( "Removed " + static_cast<string>(theDesignation) + " from the pool.");
  return true; // It's not in the pool.
}

bool Module::isEnabled() const
{
  return Module::Pool::instance()->has(designation());
}

bool Module::sendMessage(const Message& message)
{
  assert ( !message.isEmpty() );
  throw std::invalid_argument("This method has not been overridden.");
  return false;
}

bool Module::receiveMessage(const Message& message) const
{
  assert ( !message.isEmpty() );
  throw std::invalid_argument("This method has not been overridden.");
  return false;
}

bool Module::addCall(Module::Call::Ptr & callPtr)
{
  W_PRV(Module);
  auto itr = d->calls.insert(std::make_pair(callPtr->name(), callPtr));
  if (itr.second)
  {
    wdebug("Added " + callPtr->name() + " to " + static_cast<string>(designation()) + ".");
  }
  else
  {
    wwarn("Failed to add " + callPtr->name() + " to " + static_cast<string>(designation()) + ".");
  }
  return itr.second;
}

bool Module::removeCall(const string& callName)
{
  W_PRV(Module);
  Module::Call::Ptr callPtr = call(callName);
  assert (callPtr);

  if (callPtr)
  {
    auto itr = d->calls.find(callPtr->name());
    d->calls.erase(itr);
    wdebug("Removed " + callPtr->name() + " from " + static_cast<string>(designation()) + ".");
  }

  return !!callPtr;
}

Module::Call::Ptr Module::call(const string& nameOfCall) const
{
  W_PRV(const Module);
  Module::Call::Ptr callPtr = nullptr;
  Module::Call::Map::const_iterator callItr;

  wdebug("Looking up " + nameOfCall + " in " + static_cast<string>(designation()) + " from " + to_string(d->calls.size()) + " calls...");
  callItr = d->calls.find(nameOfCall);
  assert(callItr != std::end(d->calls));

  if (callItr == std::end(d->calls))
  {
    wwarn("The call " + nameOfCall + " was not found in " + static_cast<string>(designation()) + ".");
    return nullptr;
  }

  callPtr = std::dynamic_pointer_cast<Module::Call>(callItr->second);
  assert(callPtr);
  return callPtr;
}

Module::~Module()
{
}
