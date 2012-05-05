# Internal file for use of Wintermute. This file is included if you do
# find_package(Wintermute). This file automatically includes Qt's CMake
# file (UseQt4.cmake) so you need only include this file as a convenience
# wrapper.
#

#=============================================================================
# Copyright (c) 2012 Jacky Alcine <jacky.alcine@thesii.org>
#
# This module is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

include(WintermuteDefaults)
if (NOT DEFINED QT4_FOUND)
    include(FindQt4)
    include("${QT_USE_FILE}")
endif(NOT DEFINED QT4_FOUND)

##
## Prerequisites
## -----------------------------------------------------------------------------
## Define some useful variables that would be used by all of CMake.
##
##  WINTER_CURRENT_CMAKE_DIR - Defines the location at which Wintermute-specific
##                             CMake data would be stored.
##
set(WINTER_CURRENT_CMAKE_DIR "" CACHE PATH "The preferred path for Wintermute's CMake data.")

if (PROJECT_LABEL STREQUAL "Wintermute")
    set(WINTER_CURRENT_CMAKE_DIR "${CMAKE_SOURCE_DIR}/cmake")
else(PROJECT_LABEL STREQUAL "Wintermute")
    set(WINTER_CURRENT_CMAKE_DIR "${WINTER_CMAKE_DIR}")
    list(APPEND CMAKE_MODULE_PATH "${WINTER_CMAKE_DIR}")
endif(PROJECT_LABEL STREQUAL "Wintermute")

##
## O. Set up an installation target for this project.
## -----------------------------------------------------------------------------
## Now, the precarious thing about this is that we have to make sure
## that the template for un-installing is available.
##
## @note This might make a new uninstall target within a different project
##       folder even if there's a top-level project uninstall target defined.
##
if (NOT TARGET uninstall)
    set(CMAKE_UNINSTALL_TEMPLATE "${WINTER_CURRENT_CMAKE_DIR}/cmake_uninstall.cmake.in")

    configure_file("${CMAKE_UNINSTALL_TEMPLATE}"
                   "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
                   @ONLY)

    add_custom_target(uninstall
                      "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")
endif(NOT TARGET uninstall)


##
## 1. Plug-in specific work.
## -----------------------------------------------------------------------------
## Inform the build system about things that Wintermute's plug-in building might
## need in order to provide full functionality.
##