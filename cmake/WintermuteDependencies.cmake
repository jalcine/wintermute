# vim: set ts=2 sts=2 sw=2 fdm=marker
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
if (NOT DEFINED _wntr_deps)
  set(_wntr_deps ON)
else()
  return()
endif()

## ======================
## === ** PACKAGES ** ===
## ======================
## {{{ 

# = Look up package-level dependencies.
INCLUDE(FindPkgConfig)
INCLUDE(CheckIncludeFile)

PKG_SEARCH_MODULE(JsonCpp jsoncpp REQUIRED)
PKG_SEARCH_MODULE(Log4Cxx liblog4cxx REQUIRED)

# == Exported variables
set(WINTERMUTE_INCLUDE_DIRS
  ${JsonCpp_INCLUDE_DIRS}
  ${Log4Cxx_INCLUDE_DIRS}
  )

set(WINTERMUTE_LIBRARIES
  dl
  ${JsonCpp_LIBRARIES}
  ${Log4Cxx_LIBRARIES}
  )

if(DEFINED WINTERMUTE_COMPILE_FLAGS)
  set(WINTERMUTE_COMPILE_FLAGS ${WINTERMUTE_COMPILE_FLAGS}
    ${JsonCpp_CFLAGS}
    ${Log4Cxx_CFLAGS}
    )
endif()

# == Versioning
set(WINTERMUTE_VERSION_MAJOR 0)
set(WINTERMUTE_VERSION_MINOR 0)
set(WINTERMUTE_VERSION_PATCH 0)

# == Add to the required varibles to improve searching.
set(CMAKE_REQUIRED_INCLUDES ${WINTERMUTE_INCLUDE_DIRS})
set(CMAKE_REQUIRED_FLAGS ${WINTERMUTE_INCLUDE_FLAGS})

## }}}
## ======================
## ==== ** HEADERS ** ===
## ======================
## TODO: Find a better solution for looking up files and stopping the build if
## they don't exist.
## {{{

## macro find_required_header(FILE_PATH)
# Searches for the provided header at all known file paths. Wraps the
# functionality of `check_include_file` making it a hard failure.
macro(FIND_REQUIRED_HEADER _FILE_PATH)
  check_include_file(${_FILE_PATH} _found)
  if (_found-NOTFOUND)
    message(ERROR "Failed to find header: ${_FILE_PATH}!")
  endif()
endmacro(FIND_REQUIRED_HEADER)

FIND_REQUIRED_HEADER(boost/variant.hpp CHKHDR_BOOST_VARIANT)
FIND_REQUIRED_HEADER(cxxabi.h CHKHDR_CXXABI)
FIND_REQUIRED_HEADER(sys/types.h CHKHDR_SYS_TYPES)
FIND_REQUIRED_HEADER(unistd.h CHKHDR_UNISTD)

## }}}
## ======================
## ==== ** SYMBOLS ** ===
## ======================
## {{{
## }}}
