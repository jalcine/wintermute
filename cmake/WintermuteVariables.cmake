# vim: set ts=2 sts=2 sw=2 fdm=indent
###############################################################################
# Author: Jacky Alciné <me@jalcine.me>
#
# Wintermute is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# Wintermute is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with Wintermute; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
###############################################################################
if (NOT DEFINED _wntr_vars)
  set(_wntr_vars ON)
else()
  return()
endif()

SET(WINTERMUTE_COMPILE_FLAGS
  -Werror=unused
  -Wpedantic
  -Wall
  -Wunused
  )

SET(WINTERMUTE_COMPILE_FLAGS_DEBUG
  -O0
  -g3
  -Wctor-dtor-privacy
  -Wenum-compare
  -Wextra
  -Wno-conversion-null
  -Wno-deprecated
  -Wno-system-headers
  -Wno-unused-parameter
  -Woverloaded-virtual
  -Wshadow
  -Wunused-function
  -Wunused-parameter
  -Wunused-variable
  -Wwrite-strings
  -ftemplate-backtrace-limit=0
  )

SET(WINTERMUTE_COMPILE_FLAGS_RELEASE
  -O3
  )

set(WINTERMUTE_COMPILE_FLAGS ${WINTERMUTE_COMPILE_FLAGS})
set(WINTERMUTE_COMPILE_FLAGS_DEBUG "${WINTERMUTE_COMPILE_FLAGS_DEBUG}")
set(WINTERMUTE_COMPILE_FLAGS_RELEASE "${WINTERMUTE_COMPILE_FLAGS_RELEASE}")

SET(WINTERMUTE_COMPILE_DEFINITIONS WINTERMUTE)
SET(WINTERMUTE_COMPILE_DEFINITIONS_DEBUG 
  WINTERMUTE_DEBUG
  GLIBCXX_FORCE_NEW
  )

if (CMAKE_BUILD_TYPE STREQUAL Debug)
  list(APPEND WINTERMUTE_COMPILE_FLAGS_DEBUG
    -fexceptions
  )
  if(CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    set(WINTERMUTE_TEST_LIBRARIES
      gcov
    )
    list(APPEND WINTERMUTE_COMPILE_FLAGS_DEBUG
      -fprofile-arcs
      -ftest-coverage
      -fabi-version=4
    )
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    list(APPEND WINTERMUTE_COMPILE_FLAGS_DEBUG
    )
  endif()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION MATCHES "^4.6")
  list(APPEND WINTERMUTE_COMPILE_FLAGS
    --std=c++0x
  )
else()
  list(APPEND WINTERMUTE_COMPILE_FLAGS
    --std=c++11
  )
endif()

include(WintermuteVersion)
