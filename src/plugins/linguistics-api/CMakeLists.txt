project(WntrLing-Api)
cmake_minimum_required(VERSION 2.8)

set(WINTERMUTE_IS_PLUGIN ON)
set(WNTRLING_MAJOR_VERSION 0)
set(WNTRLING_MINOR_VERSION 1)
set(WNTRLING_UUID "81da3bd6-bed5-4c74-aae6-44f48cd5330c")
set(WNTRLING_VERSION
    ${WNTRLING_MAJOR_VERSION}.${WNTRLING_MINOR_VERSION})

## Packages
find_package(Wintermute 0.1 REQUIRED)
find_package(WntrData 0.1 REQUIRED)
find_package(Boost 1.46 REQUIRED)

## Includes
include(WintermuteMacros)
include(WintermuteDefaults)

## Variables
set(WNTRLING_INCLUDE_DIR "${WINTER_PLUGIN_INCLUDE_INSTALL_DIR}/linguistics-api" CACHE PATH "Includes directory.")
set(WNTRLING_LIB_DIR "${WINTER_PLUGIN_INSTALL_DIR}" CACHE PATH "Libraries directory.")
set(WNTRLING_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/WntrLing" CACHE PATH "CMake directory.")
set(WNTRLING_INCLUDE_DIRS "${WINTERMUTE_INCLUDE_DIRS}"
        ${WNTRDATA_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS})
set(WNTRLING_LIBRARIES ${WNTRDATA_LIBRARIES}
	${WINTERMUTE_LIBRARIES})

set(WNTRLING_COMPILE_DEFINTIONS ${WINTERMUTE_COMPILE_DEFINITIONS})
set(WNTRLING_COMPILE_FLAGS ${WINTERMUTE_COMPILE_FLAGS})

include_directories(${WNTRLING_INCLUDE_DIRS})

wntrplugin_add_data("linguistics-api")
include(linguistics-api.cmake)
