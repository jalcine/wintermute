###############################################################################
### Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
###
### This file is free software; you can redistribute it and/or modify
### it under the terms of the GNU General Public License as published by
### the Free Software Foundation; either version 3 of the License, or
### (at your option) any later version.
###
### This file is distributed in the hope that it will be useful,
### but WITHOUT ANY WARRANTY; without even the implied warranty of
### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
### GNU General Public License for more details.
###
### You should have received a copy of the GNU General Public License
### along with this file.  If not, see <http://www.gnu.org/licenses/>.
###############################################################################

if (NOT DEFINED GCOV_PATH)
  find_file(GCOV_PATH gcov)
endif(NOT DEFINED GCOV_PATH)

set(GCOV_ARGUMENTS "-a")
set(GCOV_COVERAGE_DATA_DIR ${CMAKE_BINARY_DIR}/coverage)

file(MAKE_DIRECTORY ${GCOV_COVERAGE_DATA_DIR})

macro(gcov_generate _target _base_strip)
  # Define GCov target.
  set(_gcov_target "${_target}_gcov")
  add_custom_target(${_gcov_target}
    DEPENDS ${_target}
    COMMENT "Collecting coverage data for ${_target}.."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

  # Run the program.
  add_custom_command(TARGET ${_gcov_target}
    COMMENT "Running executable ${_target}..."
    COMMAND ${_target}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

  # Get the sources of the specified target.
  get_target_property(_target_srcs ${_target} SOURCES)

  foreach(_target_src ${_target_srcs})
    get_filename_component(_source ${_target_src} NAME)
    string(REPLACE ".cpp" ".cpp.o" _object ${_source})

    set(_object_path "")
    execute_process(
      COMMAND find . -type f -name "${_object}"
      OUTPUT_VARIABLE _object_path
      OUTPUT_STRIP_TRAILING_WHITESPACE
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

    add_custom_command(TARGET ${_gcov_target}
      COMMENT "Generating GCov data for '${_target_src}'..."
      COMMAND ${GCOV_PATH} ${GCOV_ARGUMENTS} --object-file ${_object_path} ${_target_src}
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
  endforeach(_target_src ${_target_srcs})
endmacro(gcov_generate)
