project(WntrVisual-Api)
cmake_minimum_required(VERSION 2.8)

set(WINTERMUTE_IS_PLUGIN ON)
set(WNTRVISUAL_BUILD ON)
set(WNTRVISUAL_MAJOR_VERSION 0)
set(WNTRVISUAL_MINOR_VERSION 1)
set(WNTRVISUAL_UUID "5f0741d0-026b-11e1-8f4e-5999c633b9c0")
set(WNTRVISUAL_VERSION "${WNTRVISUAL_MAJOR_VERSION}.${WNTRVISUAL_MINOR_VERSION}")

list(APPEND CMAKE_MODULE_PATH "cmake/modules")

option(WNTRVISUAL_WITH_ADDONS "Build the plug-ins for WntrVisual" OFF)

include(WintermuteDefaults)
include(WintermuteMacros)

find_package(Wintermute 0.1 REQUIRED)

include("${QT_USE_FILE}")

file(GLOB WNTRVISUAL_QRC "data/ui/*.qrc")
file(GLOB WNTRVISUAL_UI "data/ui/*.ui")
file(GLOB CMAKE_MODULES "${PROJECT_SOURCE_DIR}/cmake/Modules/*.cmake")

## Main variables
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "Wintermute Visual")
set(WNTRVISUAL_LIB_DIR "${WINTER_PLUGIN_INSTALL_DIR}")
set(WNTRVISUAL_INCLUDE_DIR "${WINTER_PLUGIN_INCLUDE_INSTALL_DIR}/visual")
set(WNTRVISUAL_INCLUDE_DIRS "${WINTERMUTE_INCLUDE_DIRS}"
    ${WNTRVISUAL_INCLUDE_DIR}
    "${WINTER_PLUGIN_DATA_INSTALL_DIR}/visual"
    ${QT_INCLUDE_DIRS})
set(WNTRVISUAL_LIBRARIES ${WINTERMUTE_LIBRARIES}
    ${WNTRVISUAL_LIBRARIES})
set(WNTRVISUAL_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/WntrVisual" CACHE PATH "CMake directory.")

wntrplugin_add_data("visual-api")
include(visual-api.cmake)

if (WNTRVISUAL_WITH_ADDONS)
    add_subdirectory(addons)
endif(WNTRVISUAL_WITH_ADDONS)
