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
INCLUDE(WintermuteSourceBuild
  OPTIONAL
  RESULT_VARIABLE WINTERMUTE_IS_SOURCE_BUILD
  )

INCLUDE(CTest)

if (BUILD_TESTING)
  INCLUDE(Dart)
  INCLUDE(FindCxxTest)
  IF (NOT CXXTEST_FOUND)
    MESSAGE(ERROR "We need CxxTest for the test suite.")
    RETURN()
  ENDIF()
ENDIF()

SET(CXXTEST_TESTGEN_ARGS
  --runner=XUnitPrinter --have-eh --have-std
  )

if (DEFINED WINTERMUTE_IS_SOURCE_BUILD)
  set(WINTERMUTE_TEST_INCLUDE_DIRS
    ${WINTERMUTE_TEST_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/test/include
  )
else()
  set(WINTERMUTE_TEST_INCLUDE_DIRS
    ${WINTERMUTE_TEST_INCLUDE_DIRS}
    ${WINTERMUTE_INCLUDE_DIR}
  )
endif()

MACRO(wintermute_add_test _prefix _name _hdr)
  SET(_target ${_prefix}-${_name})
  CXXTEST_ADD_TEST(${_target} ${_target}_test.cc ${_hdr})
  WINTERMUTE_LINK_LIBRARIES(${_target})
  WINTERMUTE_ADD_TARGET_PROPERTIES(${_target})
  TARGET_LINK_LIBRARIES(${_target} wintermute-core gcov)
  TARGET_INCLUDE_DIRECTORIES(${_target} PUBLIC ${WINTERMUTE_TEST_INCLUDE_DIRS})
  TARGET_COMPILE_DEFINITIONS(${_target} PUBLIC DEBUG)
ENDMACRO(wintermute_add_test)
