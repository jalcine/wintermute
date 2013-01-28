project(Wintermute)
cmake_minimum_required(VERSION 2.8)

## MODULE DISCOVERY
## =======
## = Load up the path that at which our modules would be loaded from.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules"
                              "${CMAKE_BINARY_DIR}/cmake/Modules")

## Options for Wintermute's build.
option(WINTER_DEPRECATED_WARNINGS "Should Wintermute report warnings about deprecated methods?" ON)
option(WINTER_USE_QT_SESSIONS     "Will Wintermute utilize Qt's session storage?" OFF)

## CMAKE PRE-LOAD
## ==============
## = Ensure that our CMake environment is configured correctly.
# Set versioning information.
set(PROJECT_LABEL "Wintermute")
set(WINTERMUTE_MAJOR_VERSION 0)
set(WINTERMUTE_MINOR_VERSION 1)
set(WINTERMUTE_PATCH_VERSION 0)
set(WINTERMUTE_VERSION "${WINTERMUTE_MAJOR_VERSION}.${WINTERMUTE_MINOR_VERSION}-${WINTERMUTE_PATCH_VERSION}")
add_subdirectory(cmake)

## PACKAGES
## ========
# Wintermute provides the QtGui library only to provide support for graphical
# interfaces later into the future. This also allows for applications to display
# such GUIs when needed. This might cause a problem in the future if we want to
# make Wintermute lighter and use less libraries.
find_package(Qt4 ${WINTER_QT_VERSION} COMPONENTS
    QtCore
    QtGui
    QtDBus
REQUIRED)

## SUBDIRECTORY INCLUDES
## =====================
## As with most projects, the order of loading is important.
add_subdirectory(src)
#add_subdirectory(test)
#add_subdirectory(data)
