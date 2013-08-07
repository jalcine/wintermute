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

include(WintermuteMacros)

macro(wintermute_add_unit_test unittestname unittestsrc)
  # Define sources and moc them up.
  SET(unittest_${unittestname}_SRCS ${unittestsrc} ${WINTERMUTE_TEST_CORE_SOURCES})
  qt4_automoc(${unittest_${unittestname}_SRCS})

  # Set up the test as if it was Wintermute.
  add_executable(unittest_${unittestname} ${unittest_${unittestname}_SRCS})
  wintermute_add_properties(unittest_${unittestname})
  target_link_libraries(unittest_${unittestname} ${WINTERMUTE_TEST_LIBRARIES})

  # Configure dependencies.
  add_dependencies(unittest unittest_${unittestname})
endmacro(wintermute_add_unit_test unittestname unittestsrc)

macro(wintermute_add_coverage _target)
  ADD_CUSTOM_TARGET(${_target}_unit_coverage
    # Cleanup lcov
    COMMAND ${LCOV_PATH} --zerocounters --directory src/CMakeFiles/${_target}.dir

    # Capture test data.
    COMMAND ${LCOV_PATH} --capture --output-file lcov.capture.log --directory src/CMakeFiles/${_target}.dir

    # Run tests
    COMMAND make unittest -C ${CMAKE_BINARY_DIR}

    # Capturing lcov counters and generating report
    COMMAND ${LCOV_PATH} --directory src/CMakeFiles/${_target}.dir --capture --output-file lcov.capture.log
    COMMAND ${GENHTML_PATH} -o wintermute wintermute.info.cleaned
    COMMAND ${CMAKE_COMMAND} -E remove wintermute.info wintermute.info.cleaned

    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."
    )

  add_dependencies(${_target}_unit_coverage unittest)
endmacro(wintermute_add_coverage _target)
