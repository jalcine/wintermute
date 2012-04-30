# Internal file for use of Wintermute.
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

##
## O. Set up an installation target for this project.
## -----------------------------------------------------------------------------
## Now, the precarious thing about this is that we have to make sure
## that the template for un-installing is available.
##
if (NOT TARGET uninstall)
    set(CMAKE_UNINSTALL_TEMPLATE_LOCAL  "${CMAKE_SOURCE_DIR}/cmake")
    set(CMAKE_UNINSTALL_TEMPLATE_GLOBAL "${WINTER_CMAKE_DIR}")
    set(CMAKE_UNINSTALL_TEMPLATE_BASE   "${CMAKE_UNINSTALL_TEMPLATE_GLOBAL}")

    if (PROJECT_LABEL STREQUAL "Wintermute")
        set(CMAKE_UNINSTALL_TEMPLATE_BASE "${CMAKE_UNINSTALL_TEMPLATE_LOCAL}")
    endif()

    set(CMAKE_UNINSTALL_TEMPLATE "${CMAKE_UNINSTALL_TEMPLATE_BASE}/cmake_uninstall.cmake.in")

    configure_file("${CMAKE_UNINSTALL_TEMPLATE}"
                   "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
                   @ONLY)

    add_custom_target(uninstall
                      "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")
endif(NOT TARGET uninstall)
