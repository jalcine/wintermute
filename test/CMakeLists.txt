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

if (BUILD_TESTING)
  INCLUDE(WintermuteTestMacros)

  SET(WINTERMUTE_TEST_INCLUDE_DIRS
    ${CXXTEST_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/test/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_BINARY_DIR}/src/wintermutecore
    )

  CONFIGURE_FILE(include/test_suite.hpp.in
    ${CMAKE_CURRENT_SOURCE_DIR}/include/test_suite.hpp @ONLY)

  CONFIGURE_FILE(CTestConfig.cmake.in ${CMAKE_SOURCE_DIR}/CTestConfig.cmake
    @ONLY)

  ADD_SUBDIRECTORY(fixtures)
  ADD_SUBDIRECTORY(unit)
  ADD_SUBDIRECTORY(features)

  # Use Valgrind for memory checking.
  set(CMAKE_MEMORYCHECK_COMMAND valgrind)
  set(CMAKE_MEMORYCHECK_COMMAND_OPTIONS
    "--error-exitcode=1 --leak-check=full --show-reachable=yes --track-origins=yes -q")

  set(MEMORYCHECK_COMMAND
    "${CMAKE_MEMORYCHECK_COMMAND} ${CMAKE_MEMORYCHECK_COMMAND_OPTIONS}")
  separate_arguments(MEMCHECK_COMMAND) 
endif()

#== Handle work if we're in CI mode.
option(CI_BUILD "CI build.  Extra compilation flags will be set." OFF)

if(CI_BUILD)
  message(STATUS "CI build enabled; all warnings have been elevated to errors.")
  add_definitions(-Werror)
endif()
