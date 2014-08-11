#include <cxxtest/TestSuite.h>
#include "test_globals.hpp"

class PluginLibraryTestSuite : public CxxTest::TestSuite
{
public:
	void testLoadBinaryFromFilepath(void){} ;
	void testUnloadBinary(void){} ;
	void resolveMethodsFromBinary(void){} ;
};

class PluginVersionTestSuite : public CxxTest::TestSuite
{
public:
	void testConstruct(void){} ;
	void testAttemptDeserialization(void){} ;
	void testComparsionOperator(void){} ;
	void testEqualityOperator(void){} ;
};

class PluginTestSuite : public CxxTest::TestSuite
{
public:
	void testEnforceABI(void) {};
	void testAttemptLoad(void) {};
};
