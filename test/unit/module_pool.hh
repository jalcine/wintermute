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

#define MAX_SIZE 10

using Wintermute::Module;
using std::begin;
using std::end;
using std::make_shared;

class ModulePoolTestSuite : public CxxTest::TestSuite
{
public:
  void testFindingModule()
  {
    Module::Ptr modulePtr(new SampleModule);
    TS_ASSERT ( modulePtr->enable() );
    TS_ASSERT ( Module::Pool::instance()->has(modulePtr->designation()) );
    TS_ASSERT ( modulePtr->disable() );
  }

  void testAddingModule()
  {
    Module::Ptr modulePtr(new SampleModule);
    TS_ASSERT ( modulePtr->enable() );
    TS_ASSERT ( modulePtr->isEnabled() );
    TS_ASSERT ( Module::Pool::instance()->has(modulePtr->designation()) );

    Module::Ptr foundModulePtr = Module::Pool::instance()->find(modulePtr->designation());
    TS_ASSERT_EQUALS ( foundModulePtr->designation() , modulePtr->designation() );
    TS_ASSERT ( modulePtr->disable() );
  }

  void testRemovingModule()
  {
    Module::Ptr modulePtr(new SampleModule);
    TS_ASSERT ( modulePtr->enable() );
    TS_ASSERT ( Module::Pool::instance()->has(modulePtr->designation()) );
    TS_ASSERT ( modulePtr->disable() );
    TS_ASSERT ( !modulePtr->isEnabled() );
    TS_ASSERT ( !Module::Pool::instance()->has(modulePtr->designation()) );

    Module::Ptr foundModulePtr = Module::Pool::instance()->find(modulePtr->designation());
    TS_ASSERT ( !foundModulePtr );
  }
};
