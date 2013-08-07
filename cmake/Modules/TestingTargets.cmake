## TODO: Add proper CMake module definition here.
###############################################################################
### Copyright (C) 2013 Jacky Alcine <me@jalcine.me>
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
##
## This CMake file contains macros and logic poised at replacing the
## functionality of CTest in CMake. 
##
## Parts of this logic was borrowed by
## https://github.com/gergap/helloworld/blob/master/src/cmake/lcov.cmake

# Tell gcov to drop files at a specific location.
set(ENV{GCOV_PATH} ${CMAKE_BINARY_DIR}/coverage)

add_custom_target(coverage)

macro(generate_lcov _target)
  set(_lcov_target "lcov_${_target}")
  set(_work_dir ${CMAKE_BINARY_DIR}/test/unit/CMakeFiles/${_target}.dir)

  # Define the target's coverage target.
  add_custom_target(${_lcov_target}
    DEPENDS ${_target})

  # Make a directory for the target's coverage report.
  add_custom_command(TARGET ${_lcov_target}
    COMMAND mkdir -p coverage
    COMMENT "Making coverage directory in ${_work_dir}.."
    WORKING_DIRECTORY ${_work_dir})

  # Reset execution counts to 0.
#  add_custom_command(TARGET ${_lcov_target}
    #COMMENT "Prepping coverage collection..."
    #COMMAND lcov --directory . --zerocounters -t ${_target} --gcov-tool ${GCOV_PATH}
    #WORKING_DIRECTORY ${_work_dir})

  # Execute target.
  add_custom_command(TARGET ${_lcov_target}
    COMMENT "Executing test..."
    COMMAND "${_target}"
    WORKING_DIRECTORY ${_work_dir})

  # Collect coverage information.
  add_custom_command(TARGET ${_lcov_target}
    COMMENT "Capturing coverage data..."
    COMMAND gcov-4.6 -d -a -n --object-directory . *.gcda
    #COMMAND lcov --directory . --capture --output-file ./coverage/lcov.info --gcov-tool ${GCOV_PATH}
    WORKING_DIRECTORY ${_work_dir})

  # Generate HTML
  #add_custom_command(TARGET ${_lcov_target}
    #COMMENT "Generating HTML output..."
    #COMMAND genhtml -o ./coverage ./coverage/lcov.info
    #WORKING_DIRECTORY ${_work_dir})

  add_dependencies(coverage ${_lcov_target})
endmacro(generate_lcov _target)
