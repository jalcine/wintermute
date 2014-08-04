#include "libwintermutecore/module.hpp"
#include <cxxtest/TestSuite.h>

using Wintermute::Module;

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
