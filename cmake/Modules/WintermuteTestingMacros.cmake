## TODO: Add proper CMake module definition here.
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

include(CTest)
include(Dart)
include(WintermuteMacros)

set(WINTERMUTE_TEST_LIST_FILE "${CMAKE_BINARY_DIR}/DartTestfile.txt")
if (EXISTS ${WINTERMUTE_TEST_LIST_FILE})
  file(REMOVE ${WINTERMUTE_TEST_LIST_FILE})
endif(EXISTS ${WINTERMUTE_TEST_LIST_FILE})

macro(wintermute_add_unit_test unittestname unittestsrc)
  # Define sources and moc them up.
  SET(unittest_${unittestname}_SRCS ${unittestsrc} ${WINTERMUTE_TEST_CORE_SOURCES})
  qt4_automoc(${unittest_${unittestname}_SRCS})

  # Set up the test as if it was Wintermute.
  add_executable(unittest_${unittestname} ${unittest_${unittestname}_SRCS})
  wintermute_add_properties(unittest_${unittestname})
  target_link_libraries(unittest_${unittestname} ${WINTERMUTE_TEST_LIBRARIES})

  # Add it to the test file.
  set(_command "add_test(unittest_${unittestname}  "${CMAKE_BINARY_DIR}/test/unit/unittest_${unittestname}")
")
  # Append it to the file.
  file(APPEND ${WINTERMUTE_TEST_LIST_FILE} ${_command})

  # Configure dependencies.
  add_dependencies(unittest unittest_${unittestname})
endmacro(wintermute_add_unit_test unittestname unittestsrc)


