#############################################################################
### Copyright (C) 2011 - 2014 Jacky Alciné <me@jalcine.me>
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
#############################################################################
cmake_minimum_required (VERSION 2.8)
cmake_policy(SET CMP0017 NEW)

# Add our modules to CMake's searchable path.
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

# By default, install only runtime.
set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME Runtime)

# We need to know this (for text processing purposes).
set(WINTERMUTE_BUILD_PREFIX ${CMAKE_INSTALL_PREFIX})
set(WINTERMUTE_TEMPLATES_DIR ${CMAKE_SOURCE_DIR}/cmake/Templates)

include(CMakeUseLocalModules)
include(CMakeProvideUninstallTarget)
add_subdirectory(cmake)

## Import Wintermute's CMake infrastructure.
include(WintermuteVersioning)
include(WintermuteVariables)
include(WintermuteDependencies)
include(WintermuteMacros)
include(WintermuteDocumentation)

## Enable testing.
enable_testing()

## Include sub-directories.
add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(doc)
