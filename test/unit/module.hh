#include "libwintermutecore/module.hpp"
#include "sample_module.hpp"
#include <cxxtest/TestSuite.h>

using Wintermute::Module;
class ModuleTestSuite : public CxxTest::TestSuite
{
public:
  void testSample(void)
  {
    TS_ASSERT(1, 1);
  }
};
