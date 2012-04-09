project(WntrNtwk-Api)
cmake_minimum_required(VERSION 2.8)

set(WINTERMUTE_IS_PLUGIN ON)
set(WNTRNTWK_MAJOR_VERSION 0)
set(WNTRNTWK_MINOR_VERSION 1)
set(WNTRNTWK_UUID "6d2a54ae-043d-11e1-b46a-93253d2b7d89")
set(WNTRNTWK_VERSION
                ${WNTRNTWK_MAJOR_VERSION}.${WNTRNTWK_MINOR_VERSION})

include(WintermuteDefaults)
include(WintermuteMacros)

## Packages
find_package(Wintermute 0.1 REQUIRED)
find_package(QJson REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(QCA REQUIRED)

## Main variables
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "Wintermute Network")
set(NTWK_BROADCAST_INTERVAL 200)
set(WNTRNTWK_INCLUDE_DIR "${WINTER_PLUGIN_INCLUDE_INSTALL_DIR}/network-api" CACHE PATH "Includes directory.")
set(WNTRNTWK_LIB_DIR "${WINTER_PLUGIN_INSTALL_DIR}" CACHE PATH "Library install directory.")
set(WNTRNTWK_INCLUDE_DIRS ${WNTRNTWK_INCLUDE_DIR}
        ${WINTERMUTE_INCLUDE_DIRS}
        ${QJSON_INCLUDE_DIR}
        ${QCA_INCLUDE_DIRS}
        ${QT_QTNETWORK_INCLUDE_DIR})
set(WNTRNTWK_LIBRARIES ${WINTERMUTE_LIBRARIES}
        ${QT_QTNETWORK_LIBRARY}
        ${QJSON_LIBRARIES}
        ${QCA_LIBRARIES}
        ${QT_QTNETWORK_LIBRARY})
set(WNTRNTWK_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/WntrNtwk" CACHE PATH "CMake directory.")
set(WNTRNTWK_COMPILE_DEFINTIONS ${WINTERMUTE_COMPILE_DEFINITIONS})
set(WNTRNTWK_COMPILE_FLAGS ${WINTERMUTE_COMPILE_FLAGS})

wntrplugin_add_data("network-api")
include(network-api.cmake)