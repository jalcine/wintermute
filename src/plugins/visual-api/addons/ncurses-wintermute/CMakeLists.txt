project(WntrVisual-NCurses)
cmake_minimum_required(VERSION 2.8)

set(WINTERMUTE_IS_PLUGIN ON)
set(WNCURSES_MAJOR_VERSION 0)
set(WNCURSES_MINOR_VERSION 0)
set(WNCURSES_MICRO_VERSION 1)
set(WNCURSES_UUID "910d44d4-028b-11e1-bba1-4dab9016640f")
set(WNCURSES_VERSION
    ${WNCURSES_MAJOR_VERSION}.${WNCURSES_MINOR_VERSION}${WNCURSES_MICRO_VERSION})

list(APPEND CMAKE_MODULE_PATH "cmake/modules")


find_package(WntrVisual 0.1 REQUIRED)
find_package(Curses REQUIRED)
find_package(Wintermute 0.1 REQUIRED)
find_package(Qt4 COMPONENTS
    QtCore
    REQUIRED)

include(WintermuteDefaults)
include(WintermuteMacros)

include("${QT_USE_FILE}")

## Main variables
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "nCurses for Wintermute")
set(WNCURSES_LIB_DIR "${WINTER_PLUGIN_INSTALL_DIR}")
set(WNCURSES_INCLUDE_DIR "${WINTER_PLUGIN_INCLUDE_INSTALL_DIR}/ncurses")
set(WNCURSES_INCLUDE_DIRS "${WINTERMUTE_INCLUDE_DIRS}"
    ${WNTRVISUAL_INCLUDE_DIRS}
    ${CURSES_INCLUDE_DIRS}
    ${QT_INCLUDE_DIRS})
set(WNCURSES_LIBRARIES ${QT_LIBRARIES}
    ${WINTERMUTE_LIBRARIES}
    ${WNTRVISUAL_LIBRARIES}
    ${CURSES_LIBRARIES})

add_subdirectory(data)
add_subdirectory(cmake)
add_subdirectory(src)