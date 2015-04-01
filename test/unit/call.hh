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
#include "wintermute-core/call.hpp"

using Wintermute::Logging;
using Wintermute::Call;

class CallTestSuite : public CxxTest::TestSuite
{
public:
  void testBuildVoidCalls()
  {
    const string args;
    Call::Ptr voidCall(new SampleVoidCall);
    const string result = voidCall->invokeFunction(args);
    TS_ASSERT ( result.empty() );
  }

  void testBuildCallsWithReturnValue()
  {
    const string value("cookie"), args;
    Call::Ptr resultCall(new SampleCallWithValue(value));
    const string result = resultCall->invokeFunction(args);
    TS_ASSERT_EQUALS ( result, value );
  }

  void testCallPermitNoParameters()
  {
    const string args = "foo";
    Call::Ptr resultCall(new SampleCallWithValue(args));
    TS_ASSERT_EQUALS ( resultCall->invokeFunction(args), args );
  }

};
