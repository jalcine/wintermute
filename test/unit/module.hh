#include "libwintermutecore/module.hpp"
#include "sample_module.hpp"
#include <cxxtest/TestSuite.h>

using Wintermute::Module;

class ModuleTestSuite : public CxxTest::TestSuite
{
public:
  void testRegisterCall(void) {}
  void testDeregisterCall(void) {}
  void testInvokeCall(void) {}
};

class ModulePoolTestSuite : public CxxTest::TestSuite
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

class ModuleDesignationTestSuite : public CxxTest::TestSuite
{
public:
  void testConstruct(void)
  {
    Module::Designation des("local", "in.wintermute.test");
    TS_ASSERT_EQUALS ( des.name(), "local" );
    TS_ASSERT_EQUALS ( des.domain(), "in.wintermute.test" );
    TS_ASSERT_EQUALS ( des.pid(), getpid() );
  }

  void testConstructWithCustomPID(void)
  {
    Module::Designation des("local", "in.wintermute.test", 300);
    TS_ASSERT_EQUALS ( des.name(), "local" );
    TS_ASSERT_EQUALS ( des.domain(), "in.wintermute.test" );
    TS_ASSERT_EQUALS ( des.pid(), 300 );
  }

  void testAttemptDeserialization(void)
  {
    const string jsonForDesignation = "{\"name\":\"wintermute\", \"domain\":\"me.jalcine\", \"pid\":\"300\"}";
    Module::Designation des(jsonForDesignation);
    TS_ASSERT_EQUALS ( des.name(), "wintermute" );
    TS_ASSERT_EQUALS ( des.domain(), "me.jalcine" );
    TS_ASSERT_EQUALS ( des.pid(), 300 );
  }

  void checkComparsionOperator(void)
  {
    Module::Designation des1("foo", "bar.nation");
    Module::Designation des2("foo", "bar.nation");
    Module::Designation des3("bar", "bar.nation");
    TS_ASSERT ( des1 == des2 );
    TS_ASSERT_EQUALS ( des1, des2 );
    TS_ASSERT ( des1 != des3 );
  }

  void testLocalByDefault(void)
  {
    Module::Designation des("foo", "bar.nation");
    TS_ASSERT ( des.pid() == getpid() );
    TS_ASSERT ( static_cast<std::string>(des).find(std::to_string(getpid())) != std::string::npos );
  }
};
