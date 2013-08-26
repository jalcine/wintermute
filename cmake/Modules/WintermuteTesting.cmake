###############################################################################
### Copyright (C) 2013 Jacky Alciné <jacky.alcine@thesii.org>
###
### This file is part of Wintermute, the extensible AI platform.
###
### Wintermute is free software; you can redistribute it and/or modify
### it under the terms of the GNU General Public License as published by
### the Free Software Foundation; either version 3 of the License, or
### (at your option) any later version.
###
### Wintermute is distributed in the hope that it will be useful,
### but WITHOUT ANY WARRANTY; without even the implied warranty of
### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
### GNU General Public License for more details.
###
### You should have received a copy of the GNU General Public License
### along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
###############################################################################

## Grab us some coverage stuff.
find_program(GCOV_PATH gcov-4.6)
find_program(LCOV_PATH lcov)
find_program(VALGRIND_PATH valgrind)
find_program(GENHTML_PATH genhtml)

# Check for gcov.
if (NOT GCOV_PATH)
  message(FATAL_ERROR "We need `gcov` for coverage information.")
endif(NOT GCOV_PATH)

# TODO: Add a check for lcov.
# TODO: Add a check for valgrind.
# TODO: Add a check for genhtml.

## Define the top-level targets for testing.
add_custom_target(test
  COMMENT "Executing test suite..."
  DEPENDS unittest
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR} VERBATIM)

add_custom_target(unittest
  COMMENT "Running unit tests..."
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

## Define the core sources and libraries for testing)
set(WINTERMUTE_TEST_INCLUDE_DIRS ${WINTERMUTE_INCLUDE_DIRS}
  ${QT_QTTEST_INCLUDE_DIR})
set(WINTERMUTE_TEST_LIBRARIES ${QT_QTTEST_LIBRARY}
  ${WINTERMUTE_LIBRARIES})
set(WINTERMUTE_TEST_ARGUMENTS "-callgrind" "-random"  "-nocrashhandler"
  "-tickcounter" "-vs" "-v2" "-vb" )

## Automatically include the testing directories.
include_directories(${WINTERMUTE_TEST_INCLUDE_DIRS})

## Make some directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test/bin)

## Include macros.
include(WintermuteTestingMacros)
