# vim: set ts=2 sts=2 sw=2 fdm=indent
###############################################################################
# Author: Jacky Alciné <me@jalcine.me>
#
# Wintermute is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# Wintermute is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with Wintermute; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
###############################################################################

include(FindCxxTest)
include(CTest)

IF (NOT CXXTEST_FOUND)
  MESSAGE(ERROR "We need CxxTest for the test suite.")
ENDIF(NOT CXXTEST_FOUND)

INCLUDE_DIRECTORIES(${CXXTEST_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_BINARY_DIR}/src/libwintermutecore)

macro(ADD_A_TEST _name _hdr)
  CXXTEST_ADD_TEST(unittest_${_name} ${_name}_test.cc ${_hdr})
  target_link_libraries(unittest_${_name} wintermutecore)
endmacro(ADD_A_TEST)

add_a_test(message ${CMAKE_SOURCE_DIR}/test/unit/message.hh)
add_a_test(designation ${CMAKE_SOURCE_DIR}/test/unit/designation.hh)
