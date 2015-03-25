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
#include <wintermutecore/module.hpp>
#include <algorithm>

using Wintermute::Module;
using std::begin;
using std::end;
using std::make_shared;
using std::dynamic_pointer_cast;

class ModulePoolTestSuite : public CxxTest::TestSuite
{
public:
  Module::Ptr modulePtr;
  short count = 1;

  void setUp()
  {
    assert(Module::Pool::instance());
    cout << endl << "============ SETUP ==============" << endl;
    TS_ASSERT ( !modulePtr );
    ++count;
    modulePtr = make_shared<SampleModule>(count);
    cout << endl << "============ END SETUP ==============" << endl;
  }

  void tearDown()
  {
    cout << endl << "============ TEARDOWN ==============" << endl;
    modulePtr.reset();
    modulePtr = nullptr;
    cout << endl << "============ END TEARDOWN ==============" << endl;
  }

  void testFindModuleInPool()
  {
    Module::Designation des = modulePtr->designation();
    TS_ASSERT ( !Module::Pool::instance()->has(des) );
  }

  void testAddModuleToPool()
  {
    Module::Ptr foundModulePtr = nullptr;
    assert(!foundModulePtr);

    TS_ASSERT ( modulePtr->enable() );
    TS_ASSERT ( modulePtr->isEnabled() );
    TS_ASSERT ( Module::Pool::instance()->has(modulePtr->designation()) );

    foundModulePtr = Module::Pool::instance()->find(modulePtr->designation());
    TS_ASSERT ( foundModulePtr );
    TS_ASSERT_EQUALS ( foundModulePtr->designation(), modulePtr->designation() );
    auto sampleModulePtr = dynamic_pointer_cast<SampleModule>(foundModulePtr);
    TS_ASSERT ( sampleModulePtr );
    TS_ASSERT ( modulePtr->disable() );
  }

  void testRemoveModuleFromPool()
  {
    Module::Ptr foundModulePtr;

    TS_ASSERT ( modulePtr->enable() );
    TS_ASSERT ( Module::Pool::instance()->has(modulePtr->designation()) );
    TS_ASSERT ( modulePtr->disable() );

    TS_ASSERT ( !modulePtr->isEnabled() );
    TS_ASSERT ( !Module::Pool::instance()->has(modulePtr->designation()) );

    foundModulePtr = Module::Pool::instance()->find(modulePtr->designation());
    TS_ASSERT ( !foundModulePtr );
  }
};
