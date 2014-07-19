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

IF (NOT CXXTEST_FOUND)
  MESSAGE(ERROR "We need CxxTest for the test suite.")
ENDIF(NOT CXXTEST_FOUND)

INCLUDE_DIRECTORIES(${CXXTEST_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_BINARY_DIR}/src/libwintermutecore)

ENABLE_TESTING()

CXXTEST_ADD_TEST(unittest_message message_test.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/unit/message.hh)
target_link_libraries(unittest_message wintermutecore)
