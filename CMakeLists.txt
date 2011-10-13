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
set(CURSES_NEED_NCURSES ON)
#find_package(QPackageKit 0.6.11 REQUIRED)
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
set(WINTERMUTE_INCLUDE_DIRS
    ${Boost_INCLUDE_DIRS}
    ${QT_INCLUDE_DIRS}
    ${CURSES_INCLUDE_DIRS}
    )

set(WINTERMUTE_LIBRARIES
    ${Boost_LIBRARIES}
    ${QT_LIBRARIES}
    ${CURSES_LIBRARIES}
    )

set(WINTERMUTE_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/Wintermute" CACHE PATH "CMake directory.")

## Include directories
include_directories(${WINTERMUTE_INCLUDE_DIRS})

## Configs

configure_file("${PROJECT_SOURCE_DIR}/cmake/WintermuteConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfig.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/cmake/WintermuteConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfigVersion.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/Wintermute.pc.in"
   "${PROJECT_BINARY_DIR}/InstallFiles/Wintermute.pc" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
    "${PROJECT_SOURCE_DIR}/src/config.hpp" @ONLY)

add_subdirectory(src)

## Installs

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/Wintermute.pc"
    DESTINATION "${WINTER_LIB_INSTALL_DIR}/pkgconfig/")

install(FILES ${CMAKE_MODULES}
    DESTINATION "${CMAKE_MODULE_DIR}")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfig.cmake"
          "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfigVersion.cmake"
    DESTINATION "${WINTERMUTE_CMAKE_DIR}")