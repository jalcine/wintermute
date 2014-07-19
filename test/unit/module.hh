#include "libwintermutecore/module.hpp"
#include <cxxtest/TestSuite.h>

using Wintermute::Module;

class ModuleDesignationTestSuite : public CxxTest::TestSuite
{
public:
  void testLocalByDefault(void)
  {
    Module::Designation aDesignation;
    TS_ASSERT( aDesignation.isLocal() == true );
  }

  void testCheckIfNull(void)
  {
    Module::Designation aDesignation;
    Module::Designation anotherDesignation("foo", "test01-wintermute.in");

    TS_ASSERT ( aDesignation.isNull() == true );
    TS_ASSERT ( anotherDesignation.isNull() == false );

  }
};

class ModuleTestSuite : public CxxTest::TestSuite
{
public:
  void testHaveFun(void)
  {
    TS_ASSERT ( true );
  }
};
