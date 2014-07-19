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

  void testCheckIfNull(void)
  {
    Designation aDesignation;
    Designation anotherDesignation("foo","test01-wintermute.in");

    TS_ASSERT ( aDesignation.isNull() == true );
    TS_ASSERT ( anotherDesignation.isNull() == false );

  }
};
