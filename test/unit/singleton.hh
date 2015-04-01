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
#include <wintermute-core/globals.hpp>

class SingletonClass
{
  W_DEF_SINGLETON(SingletonClass)
};

W_DECLARE_SINGLETON(SingletonClass)

class SingletonTestSuite : public CxxTest::TestSuite
{
public:
  void testIsASingleton(void)
  {
    auto ptr = SingletonClass::instance();
    TS_ASSERT ( ptr.get() != nullptr );

    auto otherPtr = SingletonClass::instance();
    TS_ASSERT ( ptr == otherPtr );
  }
};

