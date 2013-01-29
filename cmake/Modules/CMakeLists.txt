cmake_minimum_required(VERSION 2.8)

## Pre-fetch some values.
include(GNUInstallDirs)

## Configure the variable definition and discovery file for Wintermute.
configure_file("${CMAKE_SOURCE_DIR}/cmake/Modules/WintermuteDefaults.cmake.in"
               "${CMAKE_BINARY_DIR}/cmake/Modules/WintermuteDefaults.cmake" @ONLY)
configure_file("${CMAKE_SOURCE_DIR}/cmake/Modules/FindWintermute.cmake.in"
               "${CMAKE_BINARY_DIR}/cmake/Modules/FindWintermute.cmake" @ONLY)

## Find all of the generated and static modules.
file(GLOB WINTER_CMAKE_MODULES "${CMAKE_SOURCE_DIR}/cmake/Modules/*.cmake"
                               "${CMAKE_BINARY_DIR}/cmake/Modules/*.cmake")

## Install the modules discovered.
include(UseWintermute)
install(FILES ${WINTER_CMAKE_MODULES}
  DESTINATION "${WINTER_CMAKE_MODULES_DIR}"
  COMPONENT dev)
