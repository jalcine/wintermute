#include "libwintermutecore/module_pool.hpp"
#include "sample_module.hpp"
#include <cxxtest/TestSuite.h>
#include <iostream>

using Wintermute::Module;
using Wintermute::ModulePool;

class ModuleTestSuite : public CxxTest::TestSuite
{
public:
  void testRegisterModules(void)
  {
    SampleModule theModule;
    TS_ASSERT( ModulePool::instance()->registerModule(theModule) == true );
  }

  void testUnregisterModules(void)
  {
    SampleModule theModule;
    TS_ASSERT( ModulePool::instance()->registerModule(theModule) == true );
    TS_ASSERT_EQUALS ( ModulePool::instance()->deregisterModule(theModule.designation()), true );
  }

  void testModules(void)
  {
    for (int i = 1; i < 9; ++i)
    {
      SampleModule theModule;
      ModulePool::instance()->registerModule(theModule);
    }
    TS_ASSERT_EQUALS ( ModulePool::instance()->modules().size(), 10 );
  }
};
