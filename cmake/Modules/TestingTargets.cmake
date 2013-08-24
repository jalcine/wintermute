## TODO: Add proper CMake module definition here.
###############################################################################
### Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
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
  set(_lcov_target ${_target})
  set(_lcov_file coverage/lcov.info)
  set(_regex "build\\/test\\/test/test")
  string(REPLACE "/" "\\/" CMAKE_SOURCE_DIR_REGEX_FRIENDLY ${CMAKE_SOURCE_DIR})
  set(_lcov_file_regex "..\\/..\\/..\\//${CMAKE_SOURCE_DIR_REGEX_FRIENDLY}")

  add_custom_command(TARGET ${_lcov_target}
    COMMENT "Making coverage directory for ${_target}.."
    COMMAND mkdir -p coverage
    )

  add_custom_command(TARGET ${_lcov_target}
    COMMENT "[lcov] Cleaning up..."
    COMMAND cmake -E remove -f ${_lcov_file}
    COMMAND cmake -E remove -f `find . | grep -e gc(ov,da,no)`

  add_custom_command(TARGET ${_lcov_target}
    COMMENT "[lcov] Prepping coverage collection..."
    COMMAND ${LCOV_PATH} -q -d ${_work_dir} -z
    )

  get_target_property(_sources ${_lcov_target} SOURCES)
  foreach(_source ${_sources})
    add_custom_command(TARGET ${_lcov_target}
      COMMENT "Handling native coverage data (gcov) for source ${_source} ..."
      COMMAND ${GCOV_PATH} ${_source} -o $<TARGET_FILE_DIR:${_target}>/CMakeFiles/$<TARGET_FILE_NAME:${_target}>.dir/*.o
    )
  endforeach(_source ${_sources})

  add_custom_command(TARGET ${_lcov_target}
    COMMENT "[test] Executing test..."
    COMMAND ${_target} ${WINTERMUTE_TEST_ARGUMENTS}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

  add_custom_command(TARGET ${_lcov_target}
    COMMENT "[lcov] Analyzing coverage collection data (lcov)..."
  add_custom_command(TARGET ${_lcov_target}
    COMMENT "Cleaning up coverage data..."
    COMMAND ${LCOV_PATH} -e ${_lcov_file} \"${CMAKE_SOURCE_DIR}/*\" -o ${_lcov_file}
    COMMAND ${LCOV_PATH} -r ${_lcov_file} \"${CMAKE_BINARY_DIR}/*\" -o ${_lcov_file}
    COMMAND ${LCOV_PATH} -r ${_lcov_file} \"*.moc\" -o ${_lcov_file}
    )
    COMMAND cat ${_lcov_file}

  add_custom_command(TARGET ${_lcov_target}
    COMMENT "[report] Generating HTML output..."
    COMMAND genhtml -o ./coverage ${_lcov_file} --prefix ${CMAKE_SOURCE_DIR}
    COMMAND command_exists x-www-browser && x-www-browser ./coverage/index.html &
    )

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
