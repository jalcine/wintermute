#include "libwintermutecore/util/serializable.hpp"
#include <cxxtest/TestSuite.h>

using std::string;

using Wintermute::Util::Serializable;

class SerializableTestSuite : public CxxTest::TestSuite
{
public:
  void testConvertMapIntoString(void)
  {
    Serializable::Map aMap;
    aMap.insert(std::make_pair("foo", std::to_string(0x1)));
    aMap.insert(std::make_pair("bar", std::to_string(0x2)));
    aMap.insert(std::make_pair("baz", std::to_string(0x4)));

    const std::string theString = "{\"foo\":1, \"bar\":2, \"baz\":4}";

    //const std::string craftedString = Serializable::toString(aMap);
    //TS_ASSERT_EQUALS(theString, craftedString);
  }

  void testConvertStringToMap(void)
  {
    Serializable::Map aMap;
    aMap.insert(std::make_pair("foo", std::to_string(0x1)));
    aMap.insert(std::make_pair("bar", std::to_string(0x2)));
    aMap.insert(std::make_pair("baz", std::to_string(0x4)));

    const std::string theString = "{\"foo\":1, \"bar\":2, \"baz\":4}";

    //const Serializable::Map craftedMap = Serializable::fromString(theString);
    //TS_ASSERT_EQUALS(craftedMap, aMap);
  }
};
