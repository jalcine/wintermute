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
#include "wintermute-core/util.hh"
#include "test_suite.hpp"

using std::string;
using std::regex;

class UtilTestSuite : public CxxTest::TestSuite
{
public:
  void testGeneratesAUuid()
  {
    TS_ASSERT_DIFFERS (
      Wintermute::Util::generate_uuid(),
      string()
    );
  }

  void testJoinStringWithMutlipleItems()
  {
    const list<string> items(5, "value");
    const string delim = " ";
    const string obtainedValue = Wintermute::Util::join_string(items, delim);
    const string expectedValue = "value value value value value";

    TS_ASSERT_EQUALS (
      obtainedValue,
      expectedValue
    );
  }

  void testJoinStringWithOnlyOneItem()
  {
    const list<string> items(1, "item");
    const string delim = " ";
    const string obtainedValue = Wintermute::Util::join_string(items, delim);
    const string expectedValue = "item";

    TS_ASSERT_EQUALS (
      obtainedValue,
      expectedValue
    );
  }

  void testJoinStringEmptyWithNoItems()
  {
    const list<string> items(0, string());
    const string delim = " ";
    const string obtainedValue = Wintermute::Util::join_string(items, delim);

    TS_ASSERT_EQUALS (
      obtainedValue,
      string()
    );
  }

  void testSplitStringWithMultipleDeliminators()
  {
    const string originalString = "this is funky";
    const auto delim = regex("[ ]", regex::basic);
    const auto obtainedValue =
      Wintermute::Util::split_string(originalString, delim);

    TS_ASSERT_EQUALS (
        obtainedValue,
        list<string>({ "this", "is", "funky" })
    );
  }

  void testSplitStringWithNoDeliminators()
  {
    const string originalString = "computer";
    const auto delim = regex("[ ]", regex::basic);
    const auto obtainedValue =
      Wintermute::Util::split_string(originalString, delim);

    TS_ASSERT_EQUALS (
        obtainedValue,
        list<string>({ "computer" })
    );
  }
};
