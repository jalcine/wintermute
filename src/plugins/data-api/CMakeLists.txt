project(WntrData-Api)
cmake_minimum_required(VERSION 2.8)

set(WINTERMUTE_IS_PLUGIN ON)
set(WNTRDATA_SOURCE ON)
set(WNTRDATA_MAJOR_VERSION 0)
set(WNTRDATA_MINOR_VERSION 1)
set(WNTRDATA_UUID "a9b6b020-f4f2-11e0-be50-0800200c9a66")
set(WNTRDATA_VERSION ${WNTRDATA_MAJOR_VERSION}.${WNTRDATA_MINOR_VERSION})

## Packages
find_package(Wintermute 0.1 REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Soprano REQUIRED)
find_package(QJson REQUIRED)

include(WintermuteMacros)
include(WintermuteDefaults)

## Main variables
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "Wintermute Database")
set(PROJECT_LABEL "Wintermute Database")
set(WNTRDATA_DATA_DIR "${WINTER_PLUGIN_DATA_INSTALL_DIR}/data" CACHE PATH "The directory where Wintermute's data files will be installed.")
set(WNTRDATA_LING_DIR "ling" CACHE PATH "The name of the folder that contains the linguistics information. There should be no trailing or leading slashes.")
set(WNTRDATA_ONTO_DIR "onto" CACHE PATH "The name of the folder that contains the ontology information. There should be no trailing or leading slashes.")
set(WNTRDATA_INCLUDE_DIR "${WINTER_PLUGIN_INCLUDE_INSTALL_DIR}/data")
set(WNTRDATA_INCLUDE_DIRS ${PYTHON_INCLUDE_DIR}
        ${qjson_INCLUDE_DIR}
        ${WINTERMUTE_INCLUDE_DIRS}
        ${QT_INCLUDE_DIR}
        ${SOPRANO_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS})
set(WNTRDATA_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/WntrData")
set(WNTRDATA_LIBRARIES "${PYTHON_LIBRARIES}"
        ${QT_QTXML_LIBRARY}
        ${qjson_LIBRARIES}
        ${SOPRANO_LIBRARIES}
        ${WINTERMUTE_LIBRARIES}
        ${Boost_LIBRARIES})
set(WNTRDATA_COMPILE_DEFINTIONS ${WINTERMUTE_COMPILE_DEFINITIONS})
set(WNTRDATA_COMPILE_FLAGS ${WINTERMUTE_COMPILE_FLAGS})

add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(cmake)
