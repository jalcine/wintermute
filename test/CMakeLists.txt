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

#== Include things we'll need.
INCLUDE(FindCxxTest)
INCLUDE(CTest)
INCLUDE(WintermuteMacros)

#== CxxTest: Search and bail.
IF (NOT CXXTEST_FOUND)
  MESSAGE(ERROR "We need CxxTest for the test suite.")
  RETURN()
ENDIF(NOT CXXTEST_FOUND)

SET(WINTERMUTE_TEST_INCLUDE_DIRS
  ${CXXTEST_INCLUDE_DIR}
	${CMAKE_SOURCE_DIR}/test/include
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_BINARY_DIR}/src
  ${CMAKE_BINARY_DIR}/src/libwintermutecore
)

MACRO(ADD_A_TEST _prefix _name _hdr)
  set(_target ${_prefix}_${_name})

  CXXTEST_ADD_TEST(${_target} ${_target}_test.cc ${_hdr})

  WINTERMUTE_LINK_LIBRARIES(${_target})
  WINTERMUTE_ADD_TARGET_PROPERTIES(${_target})
  TARGET_LINK_LIBRARIES(${_target} wintermutecore)

  SET_PROPERTY(TARGET ${_target} APPEND_STRING
    PROPERTY INCLUDE_DIRECTORIES ${WINTERMUTE_TEST_INCLUDE_DIRS})
ENDMACRO(ADD_A_TEST)

CONFIGURE_FILE(include/test_suite.hpp.in
	${CMAKE_CURRENT_SOURCE_DIR}/include/test_suite.hpp @ONLY)

ADD_SUBDIRECTORY(fixtures)
ADD_SUBDIRECTORY(unit)
