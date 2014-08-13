#include "globals.hpp"
#include <cxxtest/TestSuite.h>

class SingletonClass
{
	W_DEFINE_SINGLETON(SingletonClass)
};

W_DECLARE_SINGLETON(SingletonClass)

class SingletonTestSuite : public CxxTest::TestSuite
{
public:
	void testIsASingleton(void)
	{
		auto ptr = SingletonClass::instance();
		TS_ASSERT ( ptr.get() != nullptr );
		auto otherPtr = SingletonClass::instance();
		TS_ASSERT ( ptr == otherPtr );
	}
};
