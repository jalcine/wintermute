# Internal file for use of Wintermute. This file is included if you do
# find_package(Wintermute). 
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

## Don't load me twice.
if (DEFINED WINTER_CURRENT_CMAKE_DIR)
    return()
endif()

# We know we might shadow some paths when building locally.
cmake_policy(SET CMP0017 OLD)

## PRE-MODULE LOADING
## ==================
include(WintermuteDefaults)

## PREREQUISITIES
## ==============
## = Define some useful variables that would be used by all of CMake.
##
## =  WINTER_CURRENT_CMAKE_DIR -       Defines the location at which Wintermute-specific
## =                                   CMake modules would be stored.
## 
## =  WINTER_CURRENT_CMAKE_DATA_DIR -  Defines the location at which data for Wintermute-specific
## =                                   CMake modules would be stored.
set(WINTER_CURRENT_CMAKE_DIR "${WINTER_CMAKE_DIR}" CACHE PATH "The preferred path for Wintermute's CMake modules.")
set(WINTER_CURRENT_CMAKE_DATA_DIR "${WINTER_CMAKE_DIR}" CACHE PATH "The preferred path for Wintermute's CMake data.")

if (PROJECT_LABEL STREQUAL "Wintermute")
    message(STATUS "Building Wintermute's core from sources...")
    set(WINTER_CURRENT_CMAKE_DIR "${CMAKE_SOURCE_DIR}/cmake")
    set(WINTER_CURRENT_CMAKE_DATA_DIR "${CMAKE_SOURCE_DIR}/cmake")
else(PROJECT_LABEL STREQUAL "Wintermute")
    message(STATUS "Building component for Wintermute...")
    set(WINTER_CURRENT_CMAKE_DIR "${WINTER_CMAKE_DIR}")
    set(WINTER_CURRENT_CMAKE_DATA_DIR "${WINTER_CMAKE_DATA_DIR}")
endif(PROJECT_LABEL STREQUAL "Wintermute")

list(APPEND CMAKE_MODULE_PATH "${WINTER_CURRENT_CMAKE_DIR}")

## UNINSTALLATION TARGET
## =====================
## = Now, the precarious thing about this is that we have to make sure
## = that the template for un-installing is available.
##
## = @note This might make a new uninstall target within a different project
## =       folder even if there's a top-level project uninstall target defined.
##
if (NOT TARGET uninstall)
    set(CMAKE_UNINSTALL_TEMPLATE "${WINTER_CURRENT_CMAKE_DATA_DIR}/cmake_uninstall.cmake.in")

    configure_file("${CMAKE_UNINSTALL_TEMPLATE}"
                   "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
                   @ONLY)

    add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")
endif(NOT TARGET uninstall)

## POST-MODULE LOADING
## ===================
include(WintermuteDependencies)
include(WintermuteMacros)

## PLUGIN ORIENTATED WORK
## ======================
## = Inform the build system about things that Wintermute's plug-in building might
## = need in order provide full functionality.
