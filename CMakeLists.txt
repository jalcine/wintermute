project(Wintermute)
cmake_minimum_required(VERSION 2.8)

## Modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

option(WITH_GUI          "Build Wintermute with QtGui support."         ON)
option(WITH_PLUGINS      "Build the plug-ins packaged with Wintermute." OFF)

## Wintermute-specific macros
include(WintermuteDefaults)
include(WintermuteMacros)

## Packages
find_package(PackageKitQt2 REQUIRED)
find_package(Boost COMPONENTS
    system
    program_options
    REQUIRED)
find_package(Qt4 ${WINTER_QT_VERSION} COMPONENTS
    QtCore
    QtGui
    QtXml
    QtNetwork
    QtDBus
    REQUIRED)

include("${QT_USE_FILE}")

## Main variables
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "Wintermute")
set(PROJECT_LABEL "Wintermute")
set(WINTERMUTE_MAJOR_VERSION 0)
set(WINTERMUTE_MINOR_VERSION 1)
set(WINTERMUTE_VERSION "${WINTERMUTE_MAJOR_VERSION}.${WINTERMUTE_MINOR_VERSION}")
set(WINTERMUTE_INCLUDE_DIR "${WINTER_INCLUDE_INSTALL_DIR}")
set(WINTERMUTE_INCLUDE_DIRS
    ${Boost_INCLUDE_DIRS}
    ${QT_INCLUDE_DIRS}
    ${WINTER_PLUGIN_INCLUDE_INSTALL_DIR})
set(WINTERMUTE_LIBRARIES
    ${Boost_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY})
set(WINTERMUTE_BUILD_INCLUDE_DIRS ${WINTERMUTE_INCLUDE_DIRS}
    "${PROJECT_BINARY_DIR}/src" "${PROJECT_BINARY_DIR}/src/app"
    "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/src/app")
include_directories(${WINTERMUTE_BUILD_INCLUDE_DIRS})

add_subdirectory(cmake)
add_subdirectory(src)
add_subdirectory(data)