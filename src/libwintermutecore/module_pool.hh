#include "module.hpp"
#include "module_pool.hpp"
#include "module.hh"
#include <map>

using std::map;

namespace Wintermute
{
class ModulePoolPrivate
{
public:
  typedef map < Module::Designation, Module::Ptr, std::equal_to<Module::Designation> > Map;
  Map modules;
};
}
