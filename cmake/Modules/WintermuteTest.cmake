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

INCLUDE(CTest)
INCLUDE(WintermuteTestMacros)
INCLUDE(UseWintermute)

#== Handle work if we're in CI mode.
OPTION(CI_BUILD "CI build.  Extra compilation flags will be set." OFF)

if (BUILD_TESTING)
  INCLUDE(Dart)
  INCLUDE(FindCxxTest)
  IF (NOT CXXTEST_FOUND)
    MESSAGE(ERROR "We need CxxTest for the test suite.")
    RETURN()
  ENDIF()
ENDIF()

SET(CXXTEST_TESTGEN_ARGS
  --have-eh --have-std
  )

IF (NOT CXXTEST_FOUND)
  MESSAGE(ERROR "We need CxxTest for the test suite.")
  RETURN()
ENDIF()

SET(_wntr_test_tpl ${WINTERMUTE_CMAKE_TEMPLATE_DIR}/test_runner.cpp.in)

SET(CXXTEST_TESTGEN_ARGS
  --template ${_wntr_test_tpl}
  )

LIST(APPEND WINTERMUTE_TEST_INCLUDE_DIRS
  ${CXXTEST_INCLUDE_DIR})

if (WINTERMUTE_SOURCE_BUILD)
  CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/test/include/test_suite.hpp.in
    ${CMAKE_CURRENT_SOURCE_DIR}/test_suite.hpp
    @ONLY)

  CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/test/CTestConfig.cmake.in
    ${CMAKE_SOURCE_DIR}/CTestConfig.cmake
    @ONLY)
endif()

# Use Valgrind for memory checking.
SET(CMAKE_MEMORYCHECK_COMMAND valgrind)
SET(CMAKE_MEMORYCHECK_COMMAND_OPTIONS
  "--error-exitcode=1 --leak-check=full --show-reachable=yes --track-origins=yes -q")

SET(MEMORYCHECK_COMMAND
  "${CMAKE_MEMORYCHECK_COMMAND} ${CMAKE_MEMORYCHECK_COMMAND_OPTIONS}")

SEPARATE_ARGUMENTS(MEMORYCHECK_COMMAND)

IF(CI_BUILD)
  INCLUDE(WintermuteCI)
ENDIF()
