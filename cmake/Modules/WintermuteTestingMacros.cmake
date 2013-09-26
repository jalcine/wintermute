## TODO: Add proper CMake module definition here.
###############################################################################
### Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
##
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

include(WintermuteMacros)
include(GCovMacros)

macro(wintermute_add_unit_test unittestname unittestsrc)
  # Define sources and moc them up.
  SET(unittest_${unittestname}_SRCS ${unittestsrc} ${WINTERMUTE_TEST_CORE_SOURCES})
  qt4_automoc(${unittest_${unittestname}_SRCS})

  # Set up the test as if it was Wintermute.
  add_executable(unittest_${unittestname} ${unittest_${unittestname}_SRCS})
  wintermute_add_properties(unittest_${unittestname})
  target_link_libraries(unittest_${unittestname} ${WINTERMUTE_TEST_LIBRARIES})

  # Add to CMake's test suite.
  add_test(NAME ${unittestname}
    COMMAND $<TARGET_FILE:unittest_${unittestname}> ${WINTERMUTE_TEST_ARGUMENTS})

  # Add coverage and valgrind support.
  gcov_generate(unittest_${unittestname} "test/unit")
endmacro(wintermute_add_unit_test unittestname unittestsrc)
