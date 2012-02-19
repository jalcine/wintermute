project(Wintermute)
cmake_minimum_required(VERSION 2.8)

## Modules
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

set(PROJECT_LABEL "Wintermute Core")
set(WINTERMUTE_MAJOR_VERSION 0)
set(WINTERMUTE_MINOR_VERSION 1)
set(WINTERMUTE_VERSION "${WINTERMUTE_MAJOR_VERSION}.${WINTERMUTE_MINOR_VERSION}")

## Wintermute-specific macros
include(WintermuteDefaults)
include(WintermuteMacros)

## Install prefix
set(CMAKE_INSTALL_PREFIX "/usr")

## Packages
find_package(PackageKitQt2 REQUIRED)
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

## Main variables
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "Wintermute Core")
set(PROJECT_LOGO    "${PROJECT_SOURCE_DIR}/res/WintermuteLogo.png")
set(PROJECT_DOC_CSS "${PROJECT_SOURCE_DIR}/res/doc.css")
set(WINTERMUTE_INCLUDE_DIR "${WINTER_INCLUDE_INSTALL_DIR}")
set(WINTERMUTE_INCLUDE_DIRS
    ${Boost_INCLUDE_DIRS}
    ${QT_INCLUDE_DIRS}
    ${WINTER_PLUGIN_INCLUDE_INSTALL_DIR})
set(WINTERMUTE_LIBRARIES
    ${Boost_LIBRARIES}
    ${QT_LIBRARIES})
set(WINTERMUTE_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/Wintermute" CACHE PATH "CMake directory.")

## Include directories
include_directories(${WINTERMUTE_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(cmake)
add_subdirectory(services)

## Doxygen
enable_doxygen("${PROJECT_BINARY_DIR}/doc")

## @todo Add it to a system-wide location.
install(FILES "${PROJECT_SOURCE_DIR}/Wintermute.conf"
        DESTINATION "~/.config/Synthetic Intellect Institute")