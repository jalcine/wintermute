/*
    Wintermute - A foundation for intelligent computing.
    Copyright (c) 2010 - 2015 by Jacky Alcine

    Wintermute is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    Wintermute is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with Wintermute; if not, write to the
    Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include <cxxtest/TestSuite.h>
#include <vector>
#include "wintermutecore/util/configuration.hpp"
#include "test_suite.hpp"

using std::string;
using std::vector;

using Wintermute::Util::Configuration;

class ConfigurationTestSuite : public CxxTest::TestSuite
{
public:
  void loadCfgNoThrow(Configuration::Ptr& cfg,
      const string& path = TEST_BASE_DIR "/unit/sample")
  {
    TSM_ASSERT_THROWS_NOTHING (
      "Doesn't throw an exception when fetching configuration store from "
        + path,
      cfg = Configuration::obtainStore(path)
    );
  }

  void loadCfgThrow(Configuration::Ptr& cfg,
      const string& path = TEST_BASE_DIR "/unit/sample")
  {
    TSM_ASSERT_THROWS_ANYTHING (
      "Throws an exception when fetching configuration store from " + path,
      cfg = Configuration::obtainStore(path)
    );
  }

  void runIntegerValueTest(const int& expectedValue, const string& fullKey,
    Configuration::Ptr& configuration)
  {
    const string localFullKey(fullKey + "int");
    int obtainedValue;

    TSM_ASSERT_THROWS_NOTHING (
      "Doesn't throw an expcetion when setting a value to " + localFullKey,
      configuration->set(localFullKey, expectedValue)
    );

    obtainedValue = configuration->get(localFullKey, int());

    TSM_ASSERT_EQUALS (
      "Fetch expected value from configuration at " + localFullKey + ".",
      expectedValue, obtainedValue
    );
  }

  void runFloatValueTest(const float& expectedValue, const string& fullKey,
    Configuration::Ptr& configuration)
  {
    const string localFullKey(fullKey + "float");
    float obtainedValue;

    TSM_ASSERT_THROWS_NOTHING (
      "Doesn't throw an expcetion when setting a value to " + localFullKey,
      configuration->set(localFullKey, expectedValue)
    );

    obtainedValue = configuration->get(localFullKey, float());

    TSM_ASSERT_EQUALS (
      "Fetch expected value from configuration at " + localFullKey + ".",
      expectedValue, obtainedValue
    );
  }

  void runStringValueTest(const string& expectedValue, const string& fullKey,
    Configuration::Ptr& configuration)
  {
    const string localFullKey(fullKey + "str");
    string obtainedValue;

    TSM_ASSERT_THROWS_NOTHING (
      "Doesn't throw an expcetion when setting a value to " + localFullKey,
      configuration->set(localFullKey, expectedValue)
    );

    obtainedValue = configuration->get(localFullKey, string());

    TSM_ASSERT_EQUALS (
      "Fetch expected value from configuration at " + localFullKey + ".",
      expectedValue, obtainedValue
    );
  }

  void runBooleanValueTest(const bool& expectedValue, const string& fullKey,
    Configuration::Ptr& configuration)
  {
    const string localFullKey(fullKey + "boolean");
    bool obtainedValue;

    TSM_ASSERT_THROWS_NOTHING (
      "Doesn't throw an expcetion when setting a value to " + localFullKey,
      configuration->set(localFullKey, expectedValue)
    );

    obtainedValue = configuration->get(localFullKey, bool());

    TSM_ASSERT_EQUALS (
      "Fetch expected value from configuration at " + localFullKey + ".",
      expectedValue, obtainedValue
    );
  }

  void testUsesExtraLoadPaths()
  {
    TSM_ASSERT (
      "Uses provided directories for loading configuration.",
      Configuration::addDirectory(TEST_BASE_DIR)
    );
  }

  void testReadValue()
  {
    const string expectedValue("test");
    string obtainedValue;
    const string key("test.value");
    Configuration::Ptr configuration;

    loadCfgNoThrow(configuration);

    TSM_ASSERT_THROWS_NOTHING (
      "Doesn't throw an exception when fetching value.",
      obtainedValue = configuration->get(key, string())
    );

    TSM_ASSERT_EQUALS (
      "Fetches expected value from configuration file.",
      expectedValue,
      obtainedValue
    );
  }

  void testWriteValue()
  {
    const vector<string> keysToAdd = { "this", "is.a", "bunch.of.keys" };
    const string key("test.value2");
    Configuration::Ptr configuration;

    loadCfgNoThrow(configuration);

    for (auto suffix : keysToAdd)
    {
      const string fullKey(key + "." + suffix);
      const string expectedStringValue("test");
      const int expectedIntValue = 16384;
      const bool expectedBoolValue = true;
      const float expectedFloatValue = 3.14;

      runIntegerValueTest(expectedIntValue, fullKey, configuration);
      runFloatValueTest(expectedFloatValue, fullKey, configuration);
      runStringValueTest(expectedStringValue, fullKey, configuration);
      runBooleanValueTest(expectedBoolValue, fullKey, configuration);
    }
  }
};
