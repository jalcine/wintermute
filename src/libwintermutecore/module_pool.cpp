#include "module_pool.hh"
#include "module.hpp"
#include "module_pool.hpp"
#include <algorithm>

using Wintermute::ModulePool;
using Wintermute::ModulePoolPrivate;

W_DECLARE_SINGLETON(ModulePool);

ModulePool::ModulePool() : d_ptr(new ModulePoolPrivate)
{
  W_PRV(ModulePool);
  d->modules.clear();
}

ModulePool::List ModulePool::modules() const
{
  W_PRV(ModulePool);
  ModulePool::List knownModules;

  transform(d->modules.begin(), d->modules.end(), back_inserter(knownModules),
            [](ModulePoolPrivate::Map::value_type & val)
  {
    return val.second;
  });

  return knownModules;
}

bool ModulePool::registerModule(Module& module)
{
  auto ptr = std::make_shared<Module>(module);
  return this->registerModule(ptr);
}

bool ModulePool::registerModule(Module::Ptr module)
{
  W_PRV(ModulePool);
  if (d->modules.count(module->designation()) == 0)
  {
    auto the_value = std::make_pair(module->designation(), module);
    return d->modules.insert(the_value).second == true;
  }

  return false;
}

bool ModulePool::deregisterModule(const Module::Designation& designation)
{
  W_PRV(ModulePool);
  d->modules.erase(designation);
  return true;
}
