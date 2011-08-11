project(Wintermute)
cmake_minimum_required(VERSION 2.8)
set(WINTERMUTE_MAJOR_VERSION "0")
set(WINTERMUTE_MINOR_VERSION "01")
set(WINTERMUTE_VERSION
    ${WINTERMUTE_MAJOR_VERSION}.${WINTERMUTE_MINOR_VERSION})
#
# Modules
#
list(APPEND CMAKE_MODULE_PATH "cmake/Modules")

#
# Packages
#
find_package(WntrData REQUIRED)
find_package(WntrLing REQUIRED)
find_package(WntrNtwk REQUIRED)
find_package(Boost 1.42 COMPONENTS
    system
    program_options
    REQUIRED)
find_package(Qt4 COMPONENTS
    QtCore
    QT_DONT_USE_QTGUI
    REQUIRED)

#
# WNTR SDK
#
include(WinterDefaults)
include(WinterLibsDependencies)
include(WinterMacros)
include("${QT_USE_FILE}")
#
# Files
#
file(GLOB WINTERMUTE_SHPP "include/wntr/*.hpp")
file(GLOB WINTERMUTE_SCPP "src/*.cpp")

#
# Main variables
#
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "Wintermute Core Application")
set(PROJECT_LABEL "Wintermute Core Application")
set(WINTERMUTE_BIN_INSTALL_DIR "${WINTER_BIN_INSTALL_DIR}")
set(WINTERMUTE_INCLUDE_DIRS "${WNTRDATA_INCLUDE_DIRS}"
    ${WNTRLING_INCLUDE_DIRS}
    ${WNTRNTWK_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIR}
    )
set(WINTERMUTE_LIBRARIES "${WNTRDATA_LIBRARIES}"
    ${WNTRLING_LIBRARIES}
    ${WNTRNTWK_LIBRARIES}
    ${QT_LIBRARIES}
    ${Boost_LIBRARIES}
    )
set(WINTERMUTE_INCLUDE_DIR "${WINTER_INCLUDE_INSTALL_DIR}/wntr" CACHE PATH "Includes directory.")
set(WINTERMUTE_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/Wintermute" CACHE PATH "CMake directory.")
# ^^ might be needed for plugin system.

#
# Include directories
#
include_directories(${WINTERMUTE_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/include/wntr")

#
# Configs
#
configure_file("${PROJECT_SOURCE_DIR}/cmake/WintermuteConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfig.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/cmake/WintermuteConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfigVersion.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/Wintermute.pc.in"
   "${PROJECT_BINARY_DIR}/InstallFiles/Wintermute.pc" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
    "${PROJECT_BINARY_DIR}/config.hpp")

#
# Targets
#
add_executable(wintermute
    ${WINTERMUTE_SHPP}
    ${WINTERMUTE_SCPP})

target_link_libraries(wintermute
    ${WINTERMUTE_LIBRARIES})

#
# Installs
#

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/Wintermute.pc"
    DESTINATION "${WINTER_LIB_INSTALL_DIR}/pkgconfig/")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfig.cmake"
    DESTINATION "${WINTERMUTE_CMAKE_INSTALL_DIR}")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfigVersion.cmake"
    DESTINATION "${WINTERMUTE_CMAKE_INSTALL_DIR}")

install(FILES ${CMAKE_MODULES}
    DESTINATION "${CMAKE_MODULE_DIR}")

install(FILES ${WINTERMUTE_SHPP} "${PROJECT_BINARY_DIR}/config.hpp"
    DESTINATION "${WINTERMUTE_INCLUDE_DIR}"
    COMPONENT dev)

install(TARGETS wintermute
    RUNTIME DESTINATION "${WINTERMUTE_BIN_INSTALL_DIR}"
    # Need to give permissions to write/read from '{PREFIX}' but also executable by anyone
    COMPONENT bin)
