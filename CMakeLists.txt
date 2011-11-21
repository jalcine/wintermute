project(Wintermute)
cmake_minimum_required(VERSION 2.8)
set(WINTERMUTE_MAJOR_VERSION 0)
set(WINTERMUTE_MINOR_VERSION 0)
set(WINTERMUTE_MICRO_VERSION 1)
set(WINTERMUTE_VERSION
		${WINTERMUTE_MAJOR_VERSION}.${WINTERMUTE_MINOR_VERSION}${WINTERMUTE_MICRO_VERSION})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## Modules
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

## Wintermute SDK
include(WintermuteDefaults)
include(WintermuteMacros)

## Packages
find_package(PkgConfig REQUIRED)
find_package(Boost COMPONENTS
    system
    program_options
    REQUIRED)
find_package(Qt4 COMPONENTS
    QtCore
    QtGui
    QtDBus
    REQUIRED)

include("${QT_USE_FILE}")

#----Doxygen target----#
enable_doxygen()

## Files
file(GLOB CMAKE_MODULES "${PROJECT_SOURCE_DIR}/cmake/Modules/*.cmake")

## Main variables
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "Wintermute Core")
set(PROJECT_LABEL "Wintermute Core")
set(WINTERMUTE_INCLUDE_DIR "${WINTER_INCLUDE_INSTALL_DIR}")
set(WINTERMUTE_INCLUDE_DIRS "${WINTERMUTE_INCLUDE_DIR}"
    ${Boost_INCLUDE_DIRS}
    ${QT_INCLUDE_DIRS}
    ${WINTER_PLUGIN_INCLUDE_INSTALL_DIR})

set(WINTERMUTE_LIBRARIES
    ${Boost_LIBRARIES}
    ${QT_LIBRARIES})

set(WINTERMUTE_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/Wintermute" CACHE PATH "CMake directory.")

## Include directories
include_directories(${WINTERMUTE_INCLUDE_DIRS})

## Configs
configure_file("${PROJECT_SOURCE_DIR}/cmake/WintermuteConfig.cmake.in"
                "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfig.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/cmake/WintermuteConfigVersion.cmake.in"
                "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfigVersion.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
                "${PROJECT_SOURCE_DIR}/src/config.hpp" @ONLY)

add_subdirectory(src)

## Installs

install(FILES ${CMAKE_MODULES}
        DESTINATION "${CMAKE_MODULE_DIR}")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfig.cmake"
              "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfigVersion.cmake"
        DESTINATION "${WINTERMUTE_CMAKE_DIR}")
