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

include(GNUInstallDirs)

# Set some paths to search for.
set(CMAKE_CURRENT_PROJECT_MODULES_SOURCE_DIR
  "${CMAKE_SOURCE_DIR}/cmake/Modules")
set(CMAKE_CURRENT_PROJECT_MODULES_BINARY_DIR
  "${CMAKE_BINARY_DIR}/cmake/Modules")

# Include the project-specific modules if they exists.
if (EXISTS "${CMAKE_CURRENT_PROJECT_MODULES_SOURCE_DIR}")
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_PROJECT_MODULES_SOURCE_DIR})
  message(STATUS
    "Adding '${CMAKE_CURRENT_PROJECT_MODULES_SOURCE_DIR}' to module path.")
else()
  message(STATUS "No local source 'cmake/Modules' to import into the module path.")
endif(EXISTS "${CMAKE_CURRENT_PROJECT_MODULES_SOURCE_DIR}")

if (EXISTS "${CMAKE_CURRENT_PROJECT_MODULES_BINARY_DIR}")
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_PROJECT_MODULES_BINARY_DIR})
  message(STATUS
    "Adding '${CMAKE_CURRENT_PROJECT_MODULES_BINARY_DIR}' to module path.")
else()
  message(STATUS "No local binary 'cmake/Modules' to import into the module path.")
endif(EXISTS "${CMAKE_CURRENT_PROJECT_MODULES_BINARY_DIR}")

## Environment variable path inclusion.
set(CMAKE_ENVIRONMENT_MODULE_DIR "$ENV{CMAKE_MODULE_PATH}")

if (CMAKE_ENVIRONMENT_MODULE_DIR)
  string(REPLACE ":" ";" CMAKE_ENVIRONMENT_MODULE_DIR ${CMAKE_ENVIRONMENT_MODULE_DIR})
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_ENVIRONMENT_MODULE_DIR})
  message(STATUS "Adding '${CMAKE_ENVIRONMENT_MODULE_DIR}' to module path.")
else(CMAKE_ENVIRONMENT_MODULE_DIR)
  message(STATUS "Environment variable 'CMAKE_MODULE_PATH' was unset or doesn't exist.")
endif(CMAKE_ENVIRONMENT_MODULE_DIR)


list(APPEND CMAKE_PREFIX_PATH ${CMAKE_MODULE_PATH})
