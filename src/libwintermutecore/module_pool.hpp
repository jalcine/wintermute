#ifndef WINTERMUTE_CORE_MODULE_POOL_HPP
#define WINTERMUTE_CORE_MODULE_POOL_HPP

#include "globals.hpp"
#include "module.hpp"
#include <list>

namespace Wintermute 
{
  class ModulePoolPrivate;
  class ModulePool
  {
    W_DEFINE_PRIVATE(ModulePool);
    explicit ModulePool();

    public:
      typedef std::list<Module::Ptr> List;

      Module::Ptr module(const Module::Designation& designation) const;
      List modules() const;
      bool registerModule(Module::Ptr module);
      bool registerModule(Module& module);
      bool deregisterModule(const Module::Designation& designation);

    W_DEFINE_SINGLETON(ModulePool);
  };
}

#endif
