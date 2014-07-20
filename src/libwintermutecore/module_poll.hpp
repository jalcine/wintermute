#include <unordered_map>
#include "globals.hpp"
#include "module.hpp"

namespace Wintermute 
{
  class ModulePoolPrivate;
  class ModulePool
  {
    W_DECLARE_PRIVATE(ModulePool);
    explicit ModulePool();

    public:
      typedef SharedPtr<Module> ModulePtr;

      static ModulePtr module(const Module::Designation& designation);
      static bool registerModule(ModulePtr& module);
      static bool deregisterModule(ModulePtr& module);
  };
}
