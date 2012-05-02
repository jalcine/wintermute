project(Wintermute)
cmake_minimum_required(VERSION 2.8)

## Modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

## Packages
find_package(PackageKitQt2 REQUIRED)
find_package(Qt4 ${WINTER_QT_VERSION} COMPONENTS
    QtCore
    QtGui
    QtDBus
    REQUIRED)

## Main variables
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "Wintermute")
set(PROJECT_LABEL "Wintermute")
set(WINTERMUTE_MAJOR_VERSION 0)
set(WINTERMUTE_MINOR_VERSION 1)
set(WINTERMUTE_VERSION "${WINTERMUTE_MAJOR_VERSION}.${WINTERMUTE_MINOR_VERSION}")

## Wintermute-specific macros
include(WintermuteDefaults)
include(WintermuteMacros)

## include paths.
add_subdirectory(cmake)
add_subdirectory(src)
add_subdirectory(data)