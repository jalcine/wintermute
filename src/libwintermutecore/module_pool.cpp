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
#include <algorithm>

using Wintermute::Module;
using Wintermute::ModulePoolPrivate;

W_DECLARE_SINGLETON(Module::Pool);

Module::Pool::Pool() : d_ptr(new ModulePoolPrivate)
{
  W_PRV(ModulePool);
  d->modules.clear();
}

Module::List Module::Pool::modules() const
{
  W_PRV(ModulePool);
  Module::List knownModules;

  transform(d->modules.begin(), d->modules.end(), back_inserter(knownModules),
            [](ModulePoolPrivate::Map::value_type & val)
  {
    return val.second;
  });

  return knownModules;
}

bool Module::Pool::registerModule(Module& module)
{
  auto ptr = std::make_shared<Module>(module);
  return this->registerModule(ptr);
}

bool Module::Pool::registerModule(Module::Ptr module)
{
  W_PRV(ModulePool);
  if (d->modules.count(module->designation()) == 0)
  {
    auto the_value = std::make_pair(module->designation(), module);
    return d->modules.insert(the_value).second == true;
  }

  return false;
}

bool Module::Pool::deregisterModule(const Module::Designation& designation)
{
  W_PRV(ModulePool);
  d->modules.erase(designation);
  return true;
}

Module::Pool::~Pool()
{
}
