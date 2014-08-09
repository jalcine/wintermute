#include "libwintermutecore/plugin.hpp"
#include <cxxtest/TestSuite.h>
#include "test_globals.hpp"

using Wintermute::Plugin;

class SamplePlugin : public Wintermute::Plugin
{
public:
	explicit SamplePlugin() : Wintermute::Plugin::Plugin("sample")
	{
	}
};

class PluginTestSuite : public CxxTest::TestSuite
{
public:
	void testEnforceABI(void)
	{
		Plugin::Ptr aPlugin(new SamplePlugin);
		TS_ASSERT ( aPlugin->name().empty() == false );
	}

	void testAttemptLoad(void)
	{
		string name;
		const string expected_name("sample");
		const string pluginPath(string(get_current_dir_name()) + "/build/test/fixtures/libwintermutesample.so");
		Plugin::Ptr loadedPlugin;

		TS_TRACE ( pluginPath );

		TS_ASSERT_THROWS_NOTHING (
			loadedPlugin = Plugin::loadByFilePath(pluginPath);
		);
		TS_ASSERT ( loadedPlugin.get() != nullptr );
		TS_ASSERT_THROWS_NOTHING ( 
			name = loadedPlugin->name()
		);
		TS_ASSERT_EQUALS ( name, expected_name );
	}
};
