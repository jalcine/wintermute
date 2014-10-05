/*
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with Wintermute; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "test_suite.hpp"
#include <wintermutecore/logging.hpp>
#include <wintermutecore/version.hpp>

using Wintermute::Logging;
using Wintermute::Version;

class VersionTestSuite : public CxxTest::TestSuite
{
public:
  void setUp()
  {
    DISABLE_LOGGING;
  }

  void testConvertFromStringToVersion()
  {
    TS_ASSERT_EQUALS((string) Version("3.2.1"), "3.2.1");
    TS_ASSERT_EQUALS((string) Version("2003.2.33031"), "2003.2.33031");
    TS_ASSERT_EQUALS((string) Version("010.332.33031"), "10.332.33031");
    TS_ASSERT_EQUALS((string) Version("00000.332.33031"), "0.332.33031");
  }

  void testComparsionsLessThan()
  {
    TS_ASSERT(Version("0.0.1") < Version("1.0.0"));
    TS_ASSERT(Version("0.99.0") < Version("1.0.0"));
    TS_ASSERT(Version("0.99.999") < Version("1.0.0"));
  }

  void testComparsionsGreaterThan()
  {
    TS_ASSERT(Version("4.0.1") > Version("1.0.0"));
    TS_ASSERT(Version("5.99.0") > Version("1.0.0"));
    TS_ASSERT(Version("7.99.999") > Version("1.0.0"));
  }


  void testComparsionsLessThanEquals()
  {
    TS_ASSERT(Version("0.99.0") <= Version("0.99.55"));
    TS_ASSERT(Version("0.99.999") <= Version("1.0.0"));
  }

  void testComparsionsGreaterThanEquals()
  {
    TS_ASSERT(Version("5.99.0") >= Version("1.0.0"));
    TS_ASSERT(Version("7.99.999") >= Version("1.0.0"));
  }

  void testComparsionsEquality()
  {
    TS_ASSERT(Version("5.99.0") == Version("5.99.0"));
    TS_ASSERT(Version("0001.000000.0000000") >= Version("1.0.0"));
  }

  void testComparsionsInequality()
  {
    TS_ASSERT(Version("5.99.0") != Version("5.99.4"));
    TS_ASSERT(Version("0001.003000.0000030") != Version("1.0.0"));
  }

};
