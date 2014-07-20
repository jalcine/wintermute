#include "libwintermutecore/module_pool.hpp"
#include <cxxtest/TestSuite.h>
#include <iostream>

using Wintermute::Module;
using Wintermute::ModulePool;

class ModuleTestSuite : public CxxTest::TestSuite
{
public:
  void testRegisterModules(void)
  {
    Module theModule(Module::Designation("input", "test01.wintermute.in", 3001));
    TS_ASSERT( ModulePool::instance()->registerModule(theModule) == true );
  }

  void testUnregisterModules(void)
  {
    Module theModule(Module::Designation("input", "test01.wintermute.in", 3001));
    TS_ASSERT( ModulePool::instance()->registerModule(theModule) == true );
    TS_ASSERT_EQUALS ( ModulePool::instance()->deregisterModule(theModule.designation()), true );
  }

  void testModules(void)
  {
    for (int i = 1; i < 9; ++i) {
      Module theModule(Module::Designation("input", "test01.wintermute.in", 3001));
      ModulePool::instance()->registerModule(theModule);
    }
    TS_ASSERT_EQUALS ( ModulePool::instance()->modules().size(), 10 );
  }
};
