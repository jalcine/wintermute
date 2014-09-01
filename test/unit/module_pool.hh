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
#include "libwintermutecore/module.hpp"

using Wintermute::Module;

class ModulePoolTestSuite : public CxxTest::TestSuite
{
public:
	void testRegisterModules(void)
	{
		SampleModule theModule;
		TS_ASSERT( Module::Pool::instance()->registerModule(theModule) == true );
	}

	void testUnregisterModules(void)
	{
		SampleModule theModule;
		TS_ASSERT( Module::Pool::instance()->registerModule(theModule) == true );
		TS_ASSERT_EQUALS ( Module::Pool::instance()->deregisterModule(theModule.designation()), true );
	}

	void testModules(void)
	{
		for (int i = 1; i < 9; ++i)
		{
			SampleModule theModule;
			Module::Pool::instance()->registerModule(theModule);
		}
		TS_ASSERT_EQUALS ( Module::Pool::instance()->modules().size(), 10 );
	}
};


