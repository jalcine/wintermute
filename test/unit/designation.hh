#include "libwintermutecore/designation.hpp"
#include <cxxtest/TestSuite.h>

using Wintermute::Procedure::Designation;

class DesignationTestSuite : public CxxTest::TestSuite
{
public:
  void testLocalByDefault(void)
  {
    Designation aDesignation;
    TS_ASSERT( aDesignation.isLocal() == true );
  }
};
