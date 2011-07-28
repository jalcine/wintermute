project(Wintermute)
cmake_minimum_required(VERSION 2.8)
#
# Modules
#
list(APPEND CMAKE_MODULE_PATH "cmake/Modules")

#
# Packages
#
include(FindQt4)
include(FindBoost)
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
# Wintermute SDK
#
include(WinterDefaults)
include(WinterLibsDependencies)
include(WinterMacros)
include("${QT_USE_FILE}")
#
# Files
#
file(GLOB WINTERMUTE_SHPP "include/wintermute/*.hpp")
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
    ${QT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
    )
set(WINTERMUTE_LIBRARIES "${WNTRDATA_LIBRARIES}"
    ${WNTRLING_LIBRARIES}
    ${WNTRNTWK_LIBRARIES}
    ${QT_LIBRARIES}
    ${Boost_LIBRARIES}
    )
#set(WINTERMUTE_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_DIR}/cmake/Wintermute")
# ^^ might be needed for plugin system.

#
# Include directories
#
include_directories(${WINTERMUTE_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/include/wintermute")

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
install(TARGETS wintermute
    RUNTIME DESTINATION "${WINTERMUTE_BIN_INSTALL_DIR}"
    # Need to give permissions to write/read from '{PREFIX}/wintermute' but also executable by anyone
    COMPONENT bin)
