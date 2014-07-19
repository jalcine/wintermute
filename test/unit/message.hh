#include "libwintermutecore/message.hpp"
#include <cxxtest/TestSuite.h>

class MessageTestSuite : public CxxTest::TestSuite
{
public:
  void testAddition(void)
  {
    TS_ASSERT( 1 + 1 > 1 );
    TS_ASSERT_EQUALS( 1 + 1, 2 );
  }
};
