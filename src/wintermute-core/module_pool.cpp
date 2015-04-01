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

#include "module_pool.hh"
#include "module.hpp"
#include "logging.hpp"
#include <algorithm>

using Wintermute::Module;
using Wintermute::ModulePoolPrivate;
using std::make_pair;
using std::to_string;
using std::end;
using std::begin;
using std::dynamic_pointer_cast;

W_DECLARE_SINGLETON(Module::Pool)

Module::Pool::Pool() : d_ptr(new ModulePoolPrivate)
{
  wdebug("Setting up the module pool in a clean slate...");
  W_PRV(ModulePool);
  d->modules.clear();
  wdebug("Module pool primed.");
}

Module::Ptr Module::Pool::find(const Module::Designation& designation) const
{
  W_PRV(const ModulePool);
  Module::Ptr modulePtr = nullptr;
  wdebug("Looking up module '" + (string) designation + "'...");

  if (d->modules.count(designation) != 1)
  {
    wdebug("The module " + static_cast<string>(designation) + " was not found.");
    return nullptr;
  }

  wdebug("Found the module '" + (designation.domain() + "." + designation.name()) + "'.");
  modulePtr = d->modules.at(designation)->shared_from_this();
  assert(modulePtr);
  return modulePtr;
}

Module::List Module::Pool::modules() const
{
  W_PRV(const ModulePool);
  Module::List knownModules;

  if (!d->modules.empty())
  {
    for_each(begin(d->modules), end(d->modules),
      [&](const std::pair<const Module::Designation&, const Module::Ptr&>& thePair)
    {
      assert(thePair.second);
      knownModules.push_back(thePair.second);
    });
  }

  assert ( knownModules.size() == d->modules.size() );
  return knownModules;
}

bool Module::Pool::add(Module::Ptr& modulePtr)
{
  W_PRV(ModulePool);

  assert(modulePtr);

  if (!has(modulePtr->designation()))
  {
    auto designation = modulePtr->designation();
    auto mapValue = make_pair(designation, modulePtr);
    auto returnTupleInsert = d->modules.insert(mapValue);
    const bool wasInserted = returnTupleInsert.second;

    wdebug("Was the designation " + (string) modulePtr->designation() + " inserted? " + to_string( (int) (wasInserted) ) );

    if (wasInserted)
    {
      wdebug("There are " + to_string(d->modules.size()) + " modules available.");
    }
    else
    {
      wwarn("Failed to insert " + (string) modulePtr->designation() + " into the pool.");
    }

    return wasInserted;
  }
  else
  {
    wwarn("Module '" + static_cast<string>(modulePtr->designation()) + "' already exists in the pool.");
  }

  return false;
}

bool Module::Pool::remove(const Module::Designation& designation)
{
  W_PRV(ModulePool);

  if (!has(designation))
  {
    wwarn("Module " + static_cast<string>(designation) + " was not found.");
    return false;
  }

  Module::Ptr modulePtr = find(designation);
  const int count = d->modules.erase(designation);
  wdebug("Module " + (string) designation + " found for deletion; was the module removed? (" + std::to_string(count) + ")");
  return count == 1;
}

bool Module::Pool::has(const Module::Designation& designation) const
{
  W_PRV(const ModulePool);
  const auto itr = d->modules.find(designation);
  return itr != std::end(d->modules);
}

Module::Pool::~Pool()
{
  wdebug("Removing all modules...");
  const auto moduleList = modules();

  for ( auto modulePtr : moduleList )
  {
    auto des = modulePtr->designation();
    wdebug(static_cast<string>(des) + " being removed.");
    remove(des);
    wdebug(static_cast<string>(des) + " removed for shutdown.");
    modulePtr = nullptr;
  }

  wdebug("All modules removed.");
}
