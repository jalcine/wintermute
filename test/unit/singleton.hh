#include "globals.hpp"
#include <cxxtest/TestSuite.h>

class SingletonClass {
  W_DEFINE_SINGLETON(SingletonClass);
};

W_DECLARE_SINGLETON(SingletonClass);

class SingletonTestSuite : public CxxTest::TestSuite
{
public:
  void testIsASingleton(void)
  {
    TS_ASSERT ( SingletonClass::instance().get() != nullptr );
  }
};
