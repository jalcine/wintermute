#include "libwintermutecore/module.hpp"
#include "sample_module.hpp"
#include <cxxtest/TestSuite.h>
#include <iostream>

using Wintermute::Module;
using Wintermute::Module;

class ModuleTestSuite : public CxxTest::TestSuite
{
public:
  void testRegisterModules(void)
  {
    SampleModule theModule;
    TS_ASSERT( Module::Pool::instance()->registerModule(theModule) == true );
  }

  void testUnregisterModules(void)
  {
    SampleModule theModule;
    TS_ASSERT( Module::Pool::instance()->registerModule(theModule) == true );
    TS_ASSERT_EQUALS ( Module::Pool::instance()->deregisterModule(theModule.designation()), true );
  }

  void testModules(void)
  {
    for (int i = 1; i < 9; ++i)
    {
      SampleModule theModule;
      Module::Pool::instance()->registerModule(theModule);
    }
    TS_ASSERT_EQUALS ( Module::Pool::instance()->modules().size(), 10 );
  }
};
