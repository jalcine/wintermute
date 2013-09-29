##############################################################################
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
cmake_minimum_required (VERSION 2.8)
cmake_policy(SET CMP0017 NEW)

## Include our modules in the discovery of modules and incorporate our build
## system's logic.
set(CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
set(WINTERMUTE_BUILD_PREFIX
  ${CMAKE_INSTALL_PREFIX})
set(WINTERMUTE_PLUGIN_DEFINITION_TEMPLATE
  ${CMAKE_SOURCE_DIR}/cmake/Templates/PluginDefinition.spec.in)

include(CMakeUseLocalModules)
include(CMakeProvideUninstallTarget)
add_subdirectory(cmake)

## Import Wintermute's CMake infrastructure.
include ( WintermuteVersioning )
include ( WintermuteVariables )
include ( WintermuteDependencies )
include ( WintermuteMacros )

## Include sub-directories.
add_subdirectory ( src )
add_subdirectory ( lib )
enable_testing()
add_subdirectory ( test )
#add_subdirectory(doc)
