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

# Include some modules necessary for paths.
# NOTE: These paths are going to be fixed to GNU-compliant systems for now.
include(GNUInstallDirs)

# Define some Wintermute-specific variables.
set(WINTERMUTE_VERSION              "${WINTERMUTE_MAJOR_VERSION}.${WINTERMUTE_MINOR_VERSION}.${WINTERMUTE_PATCH_VERSION}")
set(WINTERMUTE_INCLUDE_DIR          "${WINTERMUTE_BUILD_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/wintermute")
set(WINTERMUTE_CORE_INCLUDE_DIR     "${WINTERMUTE_INCLUDE_DIR}/Wintermute")
set(WINTERMUTE_LIBRARY_DIR          "${WINTERMUTE_BUILD_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set(WINTERMUTE_BIN_DIR              "${WINTERMUTE_BUILD_PREFIX}/${CMAKE_INSTALL_BINDIR}")
set(WINTERMUTE_LIBEXEC_DIR          "${WINTERMUTE_BUILD_PREFIX}/${CMAKE_INSTALL_LIBEXECDIR}")
set(WINTERMUTE_DATA_DIR             "${WINTERMUTE_BUILD_PREFIX}/${CMAKE_INSTALL_DATADIR}/wintermute")
set(WINTERMUTE_CMAKE_TEMPLATES_DIR  "${WINTERMUTE_BUILD_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Templates")
set(WINTERMUTE_CMAKE_MODULES_DIR    "${WINTERMUTE_BUILD_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules")

set(WINTERMUTE_COMPILE_DEFINITIONS           WINTERMUTE)
set(WINTERMUTE_COMPILE_DEFINITIONS_DEBUG     WINTERMUTE_DEBUG)
set(WINTERMUTE_COMPILE_DEFINITIONS_RELEASE   WINTERMUTE_RELEASE)

set(WINTERMUTE_COMPILE_FLAGS         "-std=c++11 -Wall")
set(WINTERMUTE_COMPILE_FLAGS_RELEASE "-Ofast -frtti -fwhole-program")
set(WINTERMUTE_COMPILE_FLAGS_DEBUG   "-g -O0 -Wextra -Wall -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings -Wabi -Wempty-body -Winit-self -Wnonnull -Woverflow -fprofile-arcs -ftest-coverage")
set(WINTERMUTE_LINK_FLAGS            "")
set(WINTERMUTE_LINK_FLAGS_DEBUG      "-fprofile-arcs -ftest-coverage -lgcov")
set(WINTERMUTE_LINK_FLAGS_RELEASE    "")

set(WINTERMUTE_LIBRARIES
  Log4Qt
  QtZeroMQ
  ${QT_QTCORE_LIBRARY}
  ${QJSON_LIBRARIES}
  ${QCOMMANDLINE_LIBRARIES})
set(WINTERMUTE_INCLUDE_DIRS
  ${QT_INCLUDE_DIR}
  ${QT_QTCORE_INCLUDE_DIR}
  ${LOG4QT_INCLUDE_DIRS}
  ${QTZEROMQ_INCLUDE_DIRS}
  ${QJSON_INCLUDE_DIRS}
  ${QCOMMANDLINE_INCLUDE_DIR})

set(WINTERMUTE_PLUGIN_LIBRARY_DIR    "${WINTERMUTE_LIBRARY_DIR}/Wintermute")
set(WINTERMUTE_PLUGIN_DEFINITION_DIR "${WINTERMUTE_DATA_DIR}/plugindefs")

# Clean up duplicates
list(REMOVE_DUPLICATES WINTERMUTE_LIBRARIES)
list(REMOVE_DUPLICATES WINTERMUTE_INCLUDE_DIRS)

# Change some CMake settings.
set(CMAKE_USE_RELATIVE_PATHS true)
