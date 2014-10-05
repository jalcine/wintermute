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

#define MAX_SIZE 3

using Wintermute::Module;

class ModulePoolTestSuite : public CxxTest::TestSuite
{
public:
  void tearDown(void)
  {
    Module::List aList = Module::Pool::instance()->modules();

    for (Module::Ptr ptr : aList)
    {
      assert(ptr);
      Module::Pool::instance()->remove(ptr->designation());
    }
  }

  void testAddingModules(void)
  {
    Module::Ptr theModule(new SampleModule(1000));
    TS_ASSERT ( Module::Pool::instance()->add(theModule) );

    Module::Ptr foundModule = Module::Pool::instance()->find( theModule->designation() );
    TS_ASSERT ( foundModule );
    //TS_ASSERT_EQUALS ( theModule->designation(), foundModule->designation() );
  }

  void testRemovingModules(void)
  {
    Module::Ptr theModule(new SampleModule(2000));
    Module::Designation des = theModule->designation();
    TS_ASSERT( Module::Pool::instance()->add(theModule) );

    Module::Ptr aModule(Module::Pool::instance()->find(des));
    TS_ASSERT ( aModule );
    TS_ASSERT ( Module::Pool::instance()->remove(des) );

    Module::Ptr anotherModule = Module::Pool::instance()->find(des);
    TS_ASSERT ( !anotherModule );
  }

  void testListingModules(void)
  {
    for (int i = 0; i < MAX_SIZE; ++i)
    {
      Module::Ptr theModule(new SampleModule(i + 3000));
      TS_ASSERT ( Module::Pool::instance()->add(theModule) );
    }

    TS_ASSERT_EQUALS ( Module::Pool::instance()->modules().size(), MAX_SIZE );
  }
};

