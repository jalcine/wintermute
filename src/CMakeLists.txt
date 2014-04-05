###############################################################################
### Copyright (C) 2013, 2014 Jacky Alciné <me@jalcine.me>
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
project ( Wintermute )
include ( WintermuteVersioning )

set ( WINTERMUTE_BINARY_INCLUDE_DIRS ${WINTERMUTE_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
list ( REMOVE_ITEM WINTERMUTE_BINARY_INCLUDE_DIRS ${WINTERMUTE_INCLUDE_DIR} )

# Grab the sources!
set ( WINTERMUTE_SOURCES
  ## Core
  Wintermute/version.cpp
  Wintermute/logging.cpp
  Wintermute/arguments.cpp
  Wintermute/application.cpp

  ## Plugin
  Wintermute/factory.cpp
  Wintermute/plugin.cpp
  Wintermute/plugin_process.cpp

  ## Procedure
  Wintermute/Procedure/message.hpp
  # Wintermute/Procedure/event.hpp
  Wintermute/Procedure/call.cpp
  Wintermute/Procedure/method_call.cpp
  Wintermute/Procedure/reply_call.cpp
  # Wintermute/Procedure/event_call.cpp
  Wintermute/Procedure/module_call.cpp
  Wintermute/Procedure/lambda_call.cpp

  ## Procedure/Meta
  Wintermute/Procedure/module.cpp
  Wintermute/Procedure/process_module.cpp
  Wintermute/Procedure/dispatcher.cpp
  Wintermute/Procedure/receiver.cpp

  ## Event loop
  Wintermute/Events/call.cpp
  # Wintermute/Events/event.cpp
  Wintermute/Events/call_filter.cpp
  #Wintermute/Events/reply_call_filter.cpp
  #Wintermute/Events/method_call_filter.cpp

  main.cpp
)

set ( WINTERMUTE_HEADERS
  ## Pretty headers
  Wintermute/Application
  Wintermute/Arguments
  Wintermute/Globals
  Wintermute/Logging
  Wintermute/Plugin
  Wintermute/Version
  Wintermute/Procedure/Module
  Wintermute/Procedure/ProcessModule
  Wintermute/Procedure/Call
  Wintermute/Procedure/LambdaCall
  Wintermute/Procedure/MethodCall
  Wintermute/Procedure/Dispatcher
  Wintermute/Procedure/Receiver

  ## Native headers
  Wintermute/application.hpp
  Wintermute/arguments.hpp
  Wintermute/logging.hpp
  Wintermute/version.hpp
  Wintermute/factory.hpp
  Wintermute/plugin.hpp
  Wintermute/Procedure/module.hpp
  Wintermute/Procedure/process_module.hpp
  Wintermute/Procedure/call.hpp
  Wintermute/Procedure/lambda_call.hpp
  Wintermute/Procedure/method_call.hpp
  Wintermute/Procedure/dispatcher.hpp
  Wintermute/Procedure/receiver.hpp
)

# Wrap up the sources.
file ( MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/src/Wintermute"
  "${CMAKE_BINARY_DIR}/src/Wintermute/Procedure"
  "${CMAKE_BINARY_DIR}/src/Wintermute/Events/Filters")

# Define the executable.
add_executable ( wintermute ${WINTERMUTE_SOURCES} )

# Define properties for Wintermute's executable.
set_target_properties(wintermute PROPERTIES
  FOLDER        "Wintermute/Core"
  PROJECT_LABEL "Wintermute")

# Add the necessary properties to the 'wintermute' executable.
wintermute_add_properties(wintermute)

# Generate documentation for Wintermute.
wintermute_generate_documentation(
  TARGET           wintermute
  NAME             wintermute
  VERSION          ${WINTERMUTE_VERSION}
  BRIEF            "A curious app."
  OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/wintermute-doc
  SOURCES          ${WINTERMUTE_SOURCES}
)

include_directories(${WINTERMUTE_BINARY_INCLUDE_DIRS}
  ${WINTERMUTE_INCLUDE_DIRS})
target_link_libraries(wintermute ${WINTERMUTE_LIBRARIES})

# Generate the global definition file.
configure_file(${CMAKE_SOURCE_DIR}/src/globals.hpp.in ${CMAKE_SOURCE_DIR}/src/Wintermute/Globals @ONLY)

# Install the binary.
install(TARGETS wintermute
  COMPONENT Runtime
  RUNTIME DESTINATION ${WINTERMUTE_BIN_DIR})

foreach(_HDR ${WINTERMUTE_HEADERS})
  string(FIND ${_HDR} / _dir_pos REVERSE)
  string(SUBSTRING "${_HDR}" 0 ${_dir_pos} _DIR)
  install(FILES ${_HDR}
    COMPONENT Development
    DESTINATION ${WINTERMUTE_INCLUDE_DIR}/${_DIR})
endforeach()
