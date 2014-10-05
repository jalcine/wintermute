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

W_DECLARE_SINGLETON(Module::Pool)

Module::Pool::Pool() : d_ptr(new ModulePoolPrivate)
{
  wdebug("Setting up the module pool in a clean slate...");
  W_PRV(ModulePool);
  d->modules.clear();
}

Module::Ptr Module::Pool::find(const Module::Designation& designation) const
{
  W_PRV(const ModulePool);
  wdebug("Looking for the module designated as '" + (string) designation + "'...");
  ModulePoolPrivate::Map::const_iterator itr = d->modules.find(designation);

  if (itr != d->modules.cend())
  {
    wdebug("Found the module '" + (string) designation + "'.");
    Module::Ptr modulePtr = itr->second;
    return modulePtr;
  }

  wdebug("Could not find the module '" + (string) designation + "'.");
  return nullptr;
}

Module::List Module::Pool::modules() const
{
  W_PRV(const ModulePool);
  Module::List knownModules;

  wdebug("Returning a list of " + to_string(d->modules.size()) + " modules.");

  for (auto itr = d->modules.cbegin(); itr != d->modules.cend(); itr++)
  {
    knownModules.push_back(itr->second);
  }

  assert ( knownModules.size() == d->modules.size() );

  return knownModules;
}

bool Module::Pool::add(Module::Ptr& modulePtr)
{
  W_PRV(ModulePool);
  auto mapValue = make_pair(modulePtr->designation(), modulePtr);
  auto returnTupleInsert = d->modules.insert(mapValue);
  const bool wasInserted = returnTupleInsert.second;

  wdebug("Was the designation " + (string) modulePtr->designation()
         + " inserted? " + to_string( (int) (wasInserted) ) );

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

bool Module::Pool::remove(const Module::Designation& designation)
{
  W_PRV(ModulePool);
  Module::Ptr modulePtr(find(designation));

  if (modulePtr)
  {
    const int count = d->modules.erase(designation);
    wdebug("Module " + (string) designation + " found for deletion; was the module removed? (" + string((count == 1) ? "yes" : "no") + ")");
    return count == 1;
  }

  wdebug("Module " + (string) designation + " was not found in the pool.");
  return false;
}

Module::Pool::~Pool()
{
  W_PRV(ModulePool);
  d->modules.clear();
}

