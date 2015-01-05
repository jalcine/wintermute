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
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

INCLUDE(Dart)
INCLUDE(FindCxxTest)
INCLUDE(WintermuteTestMacros)

INCLUDE(WintermuteSourceBuild
  OPTIONAL
  RESULT_VARIABLE WINTERMUTE_IS_SOURCE_BUILD
  )

IF (NOT CXXTEST_FOUND)
  MESSAGE(ERROR "We need CxxTest for the test suite.")
  RETURN()
ENDIF()

SET(WINTERMUTE_TEST_INCLUDE_DIRS
  ${CXXTEST_INCLUDE_DIR})

if (EXISTS ${WINTERMUTE_IS_SOURCE_BUILD})
  SET(WINTERMUTE_TEST_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/test/include
    ${CMAKE_SOURCE_DIR}/test
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_BINARY_DIR}/src/wintermutecore
    ${WINTERMUTE_TEST_INCLUDE_DIRS}
    )
else()
  SET(WINTERMUTE_TEST_INCLUDE_DIRS
    ${WINTERMUTE_TEST_INCLUDE_DIRS}
    ${WINTERMUTE_TEST_INCLUDE_DIR}
    )
endif()

# TODO: Make this source-specific
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/test/include/test_suite.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/test_suite.hpp
  @ONLY)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/test/CTestConfig.cmake.in
  ${CMAKE_SOURCE_DIR}/CTestConfig.cmake
  @ONLY)

# Use Valgrind for memory checking.
set(CMAKE_MEMORYCHECK_COMMAND valgrind)
set(CMAKE_MEMORYCHECK_COMMAND_OPTIONS
  "--error-exitcode=1 --leak-check=full --show-reachable=yes --track-origins=yes -q")

set(MEMORYCHECK_COMMAND
  "${CMAKE_MEMORYCHECK_COMMAND} ${CMAKE_MEMORYCHECK_COMMAND_OPTIONS}")

SEPARATE_ARGUMENTS(MEMCHECK_COMMAND) 

#== Handle work if we're in CI mode.
OPTION(CI_BUILD "CI build.  Extra compilation flags will be set." OFF)

IF(CI_BUILD)
  MESSAGE(STATUS "CI build enabled; all warnings have been elevated to errors.")
  ADD_DEFINITIONS(-Werror)
ENDIF()
