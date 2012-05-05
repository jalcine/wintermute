project(Wintermute)
cmake_minimum_required(VERSION 2.8)

## Modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

## Options for Wintermute's build.
option(WINTER_DEPRECATED_WARNINGS "Should Wintermute report warnings about deprecated methods?" ON)

## Packages
find_package(PackageKitQt2 REQUIRED)
find_package(Qt4 ${WINTER_QT_VERSION} COMPONENTS
    QtCore
    QtGui
    QtDBus
    REQUIRED)

## Main variables
set(PROJECT_LABEL "Wintermute")
set(WINTERMUTE_MAJOR_VERSION 0)
set(WINTERMUTE_MINOR_VERSION 1)
set(WINTERMUTE_VERSION "${WINTERMUTE_MAJOR_VERSION}.${WINTERMUTE_MINOR_VERSION}")

## Includes.
## There are placed here because they depend on the PROJECT_LABEL value.
include(WintermuteDefaults)
include(WintermuteMacros)

## include paths.
add_subdirectory(cmake)
add_subdirectory(src)
add_subdirectory(data)