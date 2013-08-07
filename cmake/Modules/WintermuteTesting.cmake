###############################################################################
### Copyright (C) 2013 Jacky Alcine <jacky.alcine@thesii.org>
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

include(WintermuteTestingMacros)

## Grab us some coverage stuff.
find_program(GCOV_PATH gcov)
find_program(LCOV_PATH lcov)
find_program(GENHTML_PATH genhtml)

if (NOT GCOV_PATH)
  message(FATAL_ERROR "We need `gcov` for coverage information.")
endif(NOT GCOV_PATH)

if ( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" )
  message(WARNING "Code coverage results with an optimized (non-Debug) build may be misleading" )
endif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug" )

## Release the KRAKEN!
enable_testing()

## Define the top-level targets for testing.
add_custom_target(test )
add_custom_target(unittest )

if (PROJECT_LABEL EQUAL "Wintermute")
  add_dependencies(test wintermute)
endif(PROJECT_LABEL EQUAL "Wintermute")

## Define some dependencies.
add_dependencies(unittest test)

## Define the core sources and libraries for testing)
set(WINTERMUTE_TEST_INCLUDE_DIRS ${WINTERMUTE_INCLUDE_DIRS}
  ${QT_QTTEST_INCLUDE_DIR})
set(WINTERMUTE_TEST_LIBRARIES ${QT_QTTEST_LIBRARY}
  ${WINTERMUTE_LIBRARIES})
set(WINTERMUTE_TEST_ARGUMENTS "-callgrind" "-v2" "-vb")

## Automatically include the testing directories.
include_directories(${WINTERMUTE_TEST_INCLUDE_DIRS})
