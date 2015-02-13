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

## ===========================================================================
## === ** PACKAGES ** ===
## ===========================================================================
## {{{

# = Look up package-level dependencies.
INCLUDE(FindPkgConfig)
INCLUDE(WintermuteVariables)

PKG_SEARCH_MODULE(JsonCpp jsoncpp REQUIRED)
PKG_SEARCH_MODULE(Log4Cxx liblog4cxx REQUIRED)
PKG_SEARCH_MODULE(LibUv libuv REQUIRED)
FIND_PACKAGE(LibConfig REQUIRED)

# == Exported variables
set(WINTERMUTE_INCLUDE_DIRS
  ${JsonCpp_INCLUDE_DIRS}
  ${Log4Cxx_INCLUDE_DIRS}
  ${LibUv_INCLUDE_DIRS}
  ${LIBCONFIG_INCLUDE_DIRS}
  )

set(WINTERMUTE_LIBRARIES
  dl
  ${JsonCpp_LIBRARIES}
  ${Log4Cxx_LIBRARIES}
  ${LibUv_LIBRARIES}
  ${LIBCONFIG_LIBRARIES}
  )

list(APPEND WINTERMUTE_COMPILE_FLAGS
  ${JsonCpp_CFLAGS}
  ${Log4Cxx_CFLAGS}
  ${LibUv_CFLAGS}
  ${LIBCONFIG_CLAGS}
  )

list(APPEND WINTERMUTE_LINK_FLAGS
  ${JsonCpp_LDFLAGS}
  ${Log4Cxx_LDFLAGS}
  ${LibUv_LDFLAGS}
  ${LIBCONFIG_LDFLAGS}
  )

# == Versioning
set(WINTERMUTE_VERSION_MAJOR 0)
set(WINTERMUTE_VERSION_MINOR 0)
set(WINTERMUTE_VERSION_PATCH 0)
set(WINTERMUTE_VERSION_RC    dev)

# == Add to the required varibles to improve searching.
set(CMAKE_REQUIRED_INCLUDES ${WINTERMUTE_INCLUDE_DIRS})
set(CMAKE_REQUIRED_FLAGS ${WINTERMUTE_INCLUDE_FLAGS})

## }}}
## ===========================================================================
## ==== ** FLAGS ** ===
## ===========================================================================
## {{{
INCLUDE(CheckCXXCompilerFlag)
IF (CMAKE_BUILD_TYPE STREQUAL Debug)
  CHECK_CXX_COMPILER_FLAG(-ftemplate-backtrace-limit=0 CHKFLG_TEMPLATE_BT_LIMIT)
  IF(NOT CHKFLG_TEMPLATE_BT_LIMIT)
    MESSAGE(WARNING
      '-ftemplate-backtrace-limit' is not supported.
      Stack unwinding of templated functions will be packed.
    )
  ENDIF()
ENDIF()
## }}}
