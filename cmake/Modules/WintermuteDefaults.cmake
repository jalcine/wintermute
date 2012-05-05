cmake_policy(SET CMP0017 NEW)
# Provides some vital variables that are used by Wintermute.
#
#   WINTER_BUILD_TYPE
#   WINTERMUTE_LIBRARTIES
#

#=============================================================================
# Copyright (c) 2012 Jacky Alcine <jacky.alcine@thesii.org>
#           (c) 2012 Adrian Borucki <adrian@thesii.org>
#
# This module is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

include(GNUInstallDirs)
if (NOT DEFINED QT_DEFINITIONS)
    include(FindQt4)
endif (NOT DEFINED QT_DEFINITIONS)

set(CMAKE_USE_RELATIVE_PATHS ON)

set(GENERIC_LIB_VERSION 0.0.1)
set(GENERIC_LIB_SOVERSION 0)
set(WINTER_NON_GENERIC_LIB_VERSION 0.0.1)
set(WINTER_NON_GENERIC_LIB_SOVERSION 1)

set(WINTER_BIN_INSTALL_DIR             "${CMAKE_INSTALL_FULL_BINDIR}"                 CACHE PATH "Binary installation path.")
set(WINTER_LIB_INSTALL_DIR             "${CMAKE_INSTALL_FULL_LIBDIR}"                 CACHE PATH "Library installation path.")
set(WINTER_INCLUDE_INSTALL_DIR         "${CMAKE_INSTALL_FULL_INCLUDEDIR}/wintermute"  CACHE PATH "Header installation path.")
set(WINTER_DATA_INSTALL_DIR            "${CMAKE_INSTALL_FULL_DATADIR}/wintermute"     CACHE PATH "Data installation path.")
set(WINTER_CMAKE_DIR                   "${WINTER_DATA_INSTALL_DIR}/cmake"             CACHE PATH "CMake directory for Wintermute's data.")
set(WINTER_CMAKE_DATA_DIR              "${WINTER_CMAKE_DIR}/data"                     CACHE PATH "Path to directory for CMake-related for Wintermute.")
set(WINTER_PLUGIN_INSTALL_DIR          "${WINTER_LIB_INSTALL_DIR}/wintermute"         CACHE PATH "Plugin installation path.")
set(WINTER_PLUGIN_INCLUDE_INSTALL_DIR  "${WINTER_INCLUDE_INSTALL_DIR}/plugins"        CACHE PATH "Plugin header installation path.")
set(WINTER_PLUGIN_DATA_INSTALL_DIR     "${WINTER_DATA_INSTALL_DIR}/plugins"           CACHE PATH "Plugin data installation path.")
set(WINTER_PLUGIN_SPEC_INSTALL_DIR     "${WINTER_DATA_INSTALL_DIR}/specs"             CACHE PATH "Plugin specification installation path.")

set(WINTER_LOCALE                      "en"                                           CACHE STRING "The standard locale for Wintermute to use.")
set(WINTER_QT_VERSION                  "4.7.2"                                        CACHE STRING "The minimum version of Qt to be used to build Wintermute.")
set(WINTER_IS_PLUGIN                   OFF)

set(WINTERMUTE_COMPILE_DEFINITIONS     "WINTERMUTE;"                                  CACHE STRING "Compile definitions for Wintermute.")
set(WINTERMUTE_COMPILE_FLAGS       "-std=c++0x -Wabi -Wall -Wextra -ggdb -Wctor-dtor-privacy -Wformat -Wunused -O0"
                                                                                      CACHE STRING "Compilation files to be used with Wintermute.")

set(CMAKE_RECENT_MODULE_DIR "${CMAKE_INSTALL_FULL_DATADIR}/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules")

list(APPEND CMAKE_MODULE_PATH ${WINTER_CMAKE_DIR})