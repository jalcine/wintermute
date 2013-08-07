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
## https://github.com/gergap/helloworld/blob/master/src/cmake/${LCOV_PATH}.cmake
add_custom_target(coverage)
add_custom_target(memorycheck)

add_dependencies(coverage test)
add_dependencies(memorycheck test)

macro(generate_lcov _target)
  set(_lcov_target lcov_${_target})
  set(_work_dir ${CMAKE_BINARY_DIR}/test/unit/CMakeFiles/${_target}.dir)

  # Define the target's coverage target.
  add_custom_target(${_lcov_target}
    DEPENDS ${_target})

  # Make a directory for the target's coverage report.
  add_custom_command(TARGET ${_lcov_target}
    COMMENT "Making coverage directory in ${_work_dir}.."
    COMMAND mkdir -p coverage
    WORKING_DIRECTORY ${_work_dir})

  # Reset execution counts to 0.
  add_custom_command(TARGET ${_lcov_target}
    COMMENT "Prepping coverage collection..."
    COMMAND ${LCOV_PATH} --directory . --zerocounters -t ${_target} --gcov-tool  ${GCOV_PATH} --test-name "${PROJECT_LABEL} - ${_target}"
    WORKING_DIRECTORY ${_work_dir})

  # Execute target.
  add_custom_command(TARGET ${_lcov_target}
    COMMENT "Executing test..."
    COMMAND "${_target}"
    WORKING_DIRECTORY ${_work_dir})

  # Execute coverage tool.
  add_custom_command(TARGET ${_lcov_target}
    COMMENT "Capturing coverage data..."
    COMMAND ${GCOV_PATH} -d -a -n --object-directory . *.gcda
    COMMAND ${LCOV_PATH} --directory . --capture --output-file ./coverage/lcov.info --gcov-tool ${GCOV_PATH} -b ${CMAKE_SOURCE_DIR}
    COMMAND ${LCOV_PATH} --remove ./coverage/lcov.info /usr/include/*
    COMMAND ${LCOV_PATH} -l ./coverage/lcov.info
    WORKING_DIRECTORY ${_work_dir})

  # Generate HTML
  add_custom_command(TARGET ${_lcov_target}
    COMMENT "Generating HTML output..."
    COMMAND genhtml -o ./coverage ./coverage/lcov.info
    #COMMAND x-www-browser ./coverage/index.html &
    WORKING_DIRECTORY ${_work_dir})

  add_dependencies(coverage ${_lcov_target})
endmacro(generate_lcov _target)

macro(generate_valgrind _target)
  set(_valgrind_target "valgrind_${_target}")

  add_custom_target(${_valgrind_target}
    DEPENDS ${_target})

  add_custom_command(TARGET ${_valgrind_target}
    COMMENT "Checking memory..."
    COMMAND ${VALGRIND_PATH} --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes "${_target}"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/unit)
endmacro(generate_valgrind _target)
