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

include(CTest)
include(WintermuteMacros)

set(WINTERMUTE_TEST_LIBRARIES ${WINTERMUTE_LIBRARIES}
  ${QT_QTTEST_LIBRARY})

include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/test/include
  ${QT_QTTEST_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

get_target_property(_wntr_srcs wintermute SOURCES)
set(_wntrlib_srcs )

foreach(_file ${_wntr_srcs})
  if (NOT ${_file} STREQUAL "main.cpp")
    list(APPEND _wntrlib_srcs "${CMAKE_SOURCE_DIR}/src/${_file}")
  endif()
endforeach()

add_library(wintermute-test-library STATIC ${_wntrlib_srcs})
wintermute_add_properties(wintermute-test-library)
add_dependencies(wintermute-test-library wintermute)
target_link_libraries(wintermute-test-library ${WINTERMUTE_TEST_LIBRARIES})

macro(wintermute_test_render)
  set(_singleArgs )
  set(_oneArgs   TARGET)
  set(_multiArgs SOURCES)
  cmake_parse_arguments(wtr "${_singleArgs}" "${_oneArgs}" "${_multiArgs}"
    ${ARGN})

  set(_test_name "test-${wtr_TARGET}")
  set(_test_tgt "${_test_name}")

  add_executable(${_test_tgt} ${wtr_SOURCES})
  wintermute_add_properties(${_test_tgt})
  target_link_libraries(${_test_tgt} wintermute-test-library)

  add_test(NAME "${_test_name}-driver"
    COMMAND $<TARGET_FILE_DIR:${_test_tgt}>/$<TARGET_FILE_NAME:${_test_tgt}>)
endmacro()
