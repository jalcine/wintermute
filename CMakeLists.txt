project(Wintermute)
cmake_minimum_required(VERSION 2.8)
set(WINTERMUTE_MAJOR_VERSION 0)
set(WINTERMUTE_MINOR_VERSION 1)
set(WINTERMUTE_MICRO_VERSION 0)
set(WINTERMUTE_VERSION
    ${WINTERMUTE_MAJOR_VERSION}.${WINTERMUTE_MINOR_VERSION}.${WINTERMUTE_MICRO_VERSION})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#
# Modules
#
list(APPEND CMAKE_MODULE_PATH "cmake/Modules")

#
# Packages
#
set(CURSES_NEED_NCURSES TRUE)
find_package(WntrData REQUIRED)
find_package(WntrLing REQUIRED)
find_package(WntrNtwk REQUIRED)
find_package(Curses REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Boost 1.42 COMPONENTS
    system
    program_options
    REQUIRED)
find_package(Qt4 4.4.3 COMPONENTS
    QtCore
    QtGui
    QtDBus
    REQUIRED)

#
# WNTR SDK
#
include(WintermuteMacros)
include("${QT_USE_FILE}")

#----Doxygen target----#
enable_doxygen()

#
# Files
#
file(GLOB WINTERMUTE_SHPP "include/wntr/*.hpp")
file(GLOB WINTERMUTE_SCPP "src/*.cpp")
file(GLOB CMAKE_MODULES "${PROJECT_SOURCE_DIR}/cmake/Modules/*.cmake")

#
# Main variables
#
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "Wintermute Core")
set(PROJECT_LABEL "Wintermute Core")

set(WINTERMUTE_USE_GUI false CACHE BOOL "Determines whether or not Wintermute is using an GUI base.")
set(WINTERMUTE_BIN_INSTALL_DIR bin)
set(WINTERMUTE_LIB_INSTALL_DIR lib)
set(WINTERMUTE_INCLUDE_DIRS 
    ${WNTRLING_INCLUDE_DIRS}
    ${WNTRDATA_INCLUDE_DIRS}
    ${WNTRNTWK_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${QT_INCLUDE_DIRS}
    ${CURSES_INCLUDE_DIRS}
    )
set(WINTERMUTE_LIBRARIES 
    ${WNTRLING_LIBRARIES}
    ${WNTRDATA_LIBRARIES}
    ${WNTRNTWK_LIBRARIES}
    ${Boost_LIBRARIES}
    ${QT_LIBRARIES}
    ${CURSES_LIBRARIES}
    )

set(WINTERMUTE_INCLUDE_DIR "include/wntr/core" CACHE PATH "Includes directory.")
set(WINTERMUTE_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/Wintermute" CACHE PATH "CMake directory.")

if(WINTERMUTE_USE_GUI)
    set(WINTERMUTE_INCLUDE_DIRS ${WINTERMUTE_INCLUDE_DIRS} ${QT_QTGUI_INCLUDE_DIR})
    set(WINTERMUTE_LIBRARIES ${WINTERMUTE_LIBRARIES} ${QT_QTGUI_LIBRARY})
    set(WINTERMUTE_USE_GUI "true")
else()
    set(WINTERMUTE_USE_GUI "false")
endif()

#
# Include directories
#
include_directories(${WINTERMUTE_INCLUDE_DIRS})

#
# Configs
#

configure_file("${PROJECT_SOURCE_DIR}/Wintermute.pc.in"
   "${PROJECT_BINARY_DIR}/InstallFiles/Wintermute.pc" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.hpp")

add_subdirectory(src)
#
# Installs
#

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/Wintermute.pc"
    DESTINATION "${WINTERMUTE_LIB_INSTALL_DIR}/pkgconfig/")

