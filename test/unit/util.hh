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
		aMap.insert(std::make_pair("bar", std::to_string(0x2)));
		aMap.insert(std::make_pair("baz", std::to_string(0x4)));
		aMap.insert(std::make_pair("foo", std::to_string(0x1)));

		const std::string theString = "{\"bar\":\"2\",\"baz\":\"4\",\"foo\":\"1\"}\n";

		const std::string craftedString = Serializable::toString(aMap);
		TS_ASSERT_EQUALS(theString, craftedString);
	}

	void testConvertStringToMap(void)
	{
		Serializable::Map aMap;
		aMap.insert(std::make_pair("foo", std::to_string(0x2)));
		aMap.insert(std::make_pair("bar", std::to_string(0x3)));
		aMap.insert(std::make_pair("baz", std::to_string(0x5)));

		const std::string theString = "{\"foo\":2, \"bar\":3, \"baz\":5}";

		const Serializable::Map craftedMap = Serializable::fromString(theString);
		TS_ASSERT_EQUALS(craftedMap, aMap);
	}
};
