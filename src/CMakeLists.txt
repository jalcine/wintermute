cmake_minimum_required(VERSION 2.8)
project(WintermuteApplication)

include(WintermuteDefaults)
include(WintermuteMacros)

## Configures
configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_SOURCE_DIR}/config.hpp" @ONLY)

file(GLOB WINTERMUTE_SOURCES "*.cpp")
file(GLOB WINTERMUTE_HEADERS "*.hpp")

include("${QT_USE_FILE}")

set(WINTERMUTE_INCLUDE_DIR "${WINTER_INCLUDE_INSTALL_DIR}")
set(WINTERMUTE_INCLUDE_DIRS ${WINTERMUTE_INCLUDE_DIR}
    ${QT_INCLUDES}
    ${WINTER_PLUGIN_INCLUDE_INSTALL_DIR})
set(WINTERMUTE_LIBRARIES
    ${QT_LIBRARIES})
set(WINTERMUTE_BUILD_INCLUDE_DIRS ${WINTERMUTE_INCLUDE_DIRS}
    "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}")

include_directories(${WINTERMUTE_BUILD_INCLUDE_DIRS})

QT4_AUTOMOC(${WINTERMUTE_SOURCES})

## make me some docs, will ya?
wntr_make_docs(PATH "${PROJECT_SOURCE_DIR}")

## Targets
add_executable(wintermute ${WINTERMUTE_SOURCES})
target_link_libraries(wintermute ${WINTERMUTE_LIBRARIES})
set_target_properties(wintermute PROPERTIES
    COMPILE_DEFINITIONS "${WINTERMUTE_COMPILE_DEFINITIONS}"
    COMPILE_FLAGS ${WINTERMUTE_COMPILE_FLAGS})

## Installs
install(TARGETS wintermute
    RUNTIME DESTINATION "${WINTER_BIN_INSTALL_DIR}"
    COMPONENT bin)

install(FILES ${WINTERMUTE_HEADERS}
        DESTINATION "${WINTER_INCLUDE_INSTALL_DIR}"
        COMPONENT dev)

# need the variables here for this module.
configure_file("${CMAKE_SOURCE_DIR}/cmake/Modules/FindWintermute.cmake.in"
               "${CMAKE_BINARY_DIR}/InstallFiles/FindWintermute.cmake" @ONLY)