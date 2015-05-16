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
#include <wintermute-core/module.hpp>
#include <wintermute-core/logging.hpp>

using Wintermute::Module;
using Wintermute::Call;

class ModuleTestSuite : public CxxTest::TestSuite
{
public:

  void testHasDesignation(void)
  {
    Module::Ptr modulePtr(new SampleModule);
    TS_ASSERT ( !modulePtr->designation().isNull() );
  }

  // FIXME Finish this test.
  void testCanEnableModule(void) { }

  // FIXME Finish this test.
  void testCanDisableModule(void) { }

  void testAddCallToModule()
  {
    const string expectedValue("CookieMonster");
    Module::Ptr modulePtr(new SampleModule);
    Module::Call::Ptr resultCall(new SampleModuleCallWithValue(modulePtr, expectedValue));

    TS_ASSERT ( modulePtr->addCall(resultCall) );
    Module::Call::Ptr obtainedCall = modulePtr->call(resultCall->name());
    TS_ASSERT ( obtainedCall );
    TS_ASSERT_EQUALS ( obtainedCall, resultCall );

    string arguments;
    const string result = obtainedCall->invokeFunction(arguments);
    TS_ASSERT_EQUALS ( result, expectedValue );
  }

  void testRemoveCallFromModule()
  {
    Module::Ptr modulePtr(new SampleModule);
    Module::Call::Ptr resultCall(new SampleModuleCallWithValue(modulePtr));

    TS_ASSERT ( modulePtr->addCall(resultCall) );
    TS_ASSERT ( modulePtr->removeCall(resultCall->name()) );
    TS_ASSERT ( resultCall );
  }
};

