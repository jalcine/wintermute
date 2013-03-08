###############################################################################
### Copyright (C) 2013 Jacky Alcine <jacky.alcine@thesii.org>
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

cmake_minimum_required(VERSION 2.8)
project(Wintermute)

## Include our modules in the discovery of modules.
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
include(CMakeUseLocalModules)
include(CMakeProvideUninstallTarget)
include(WintermuteMacros)
include(WintermuteTesting)

## Define some project-specific variables.
set(WINTERMUTE_VERSION_MAJOR "0")
set(WINTERMUTE_VERSION_MINOR "1")
set(WINTERMUTE_VERSION_PATCH "1")

## Include sub-directories.
add_subdirectory(cmake)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(doc)
