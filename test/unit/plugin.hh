#include <algorithm>
#include <cxxtest/TestSuite.h>
#include "libwintermutecore/plugin.hpp"
#include "test_globals.hpp"

using Wintermute::Plugin;

class PluginLibraryTestSuite : public CxxTest::TestSuite
{
public:
	void testLoadBinaryFromFilepath(void)
	{
	}
	void testUnloadBinary(void)
	{
	}
	void resolveMethodsFromBinary(void)
	{
	}
};

class PluginTestSuite : public CxxTest::TestSuite
{
public:
	void testAttemptLoad(void)
	{
		Plugin::Ptr pluginPtr = Plugin::load("sample");
		const list<string> plugins = Plugin::loadedPlugins();
		TS_ASSERT(std::find(plugins.begin(), plugins.end(), "sample") != std::end(plugins));
		TS_ASSERT (pluginPtr);
	}
	void testAttemptUnload(void)
	{
		Plugin::Ptr pluginPtr = Plugin::load("sample");
		TS_ASSERT(Plugin::unload("sample"));
		TS_ASSERT(!pluginPtr);
	}
};
