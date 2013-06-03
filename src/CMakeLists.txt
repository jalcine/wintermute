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

include(WintermuteVersioning)

set(WINTERMUTE_BUILD_INCLUDE_DIRS ${WINTERMUTE_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
list(REMOVE_ITEM WINTERMUTE_BUILD_INCLUDE_DIRS ${WINTERMUTE_INCLUDE_DIR})

# Grab the sources!
set(WINTERMUTE_SOURCES
  Wintermute/application.cpp
  Wintermute/arguments.cpp
  Wintermute/logging.cpp
  Wintermute/plugin.cpp
  Wintermute/plugininterfaceable.cpp
  Wintermute/factory.cpp
  Wintermute/temporaryplugin.cpp
  Wintermute/version.cpp
  main.cpp
  )

set(WINTERMUTE_HEADERS 
  ### Pretty headers
  Wintermute/Application
  Wintermute/Arguments
  Wintermute/Globals
  Wintermute/Logging
  Wintermute/Plugin
  Wintermute/PluginInterfaceable
  Wintermute/Version

  ### Native headers
  Wintermute/application.hpp
  Wintermute/arguments.hpp
  Wintermute/logging.hpp
  Wintermute/plugin.hpp
  Wintermute/version.hpp
  Wintermute/factory.hpp
  Wintermute/plugininterfaceable.hpp
  "${CMAKE_CURRENT_BINARY_DIR}/globals.hpp"
  )

# Wrap up the sources.
qt4_automoc(${WINTERMUTE_SOURCES})

# Define the executable.
add_executable(wintermute ${WINTERMUTE_SOURCES})

# Define properties for Wintermute's executable.
set_target_properties(wintermute PROPERTIES
  FOLDER "Wintermute/Core"
  VERSION "${WINTERMUTE_VERSION}"
  PROJECT_LABEL "Wintermute")

# Add the necessary properties to the 'wintermute' executable.
wintermute_add_properties(wintermute)

include_directories(${WINTERMUTE_BUILD_INCLUDE_DIRS})
target_link_libraries(wintermute ${WINTERMUTE_LIBRARIES})

# Generate the global definition file.
configure_file(globals.hpp.in globals.hpp @ONLY)

# Install the binary.
install(TARGETS wintermute
  RUNTIME DESTINATION ${WINTERMUTE_BIN_DIR})        

install(FILES ${WINTERMUTE_HEADERS}
  DESTINATION ${WINTERMUTE_CORE_INCLUDE_DIR})
