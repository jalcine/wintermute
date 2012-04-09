project(WntrVisual-Nlui)
cmake_minimum_required(VERSION 2.8)

set(WINTERMUTE_IS_PLUGIN ON)
set(WNTRNLUI_MAJOR_VERSION 0)
set(WNTRNLUI_MINOR_VERSION 0)
set(WNTRNLUI_MICRO_VERSION 1)
set(WNTRNLUI_UUID "e930a2a4-0502-11e1-b72f-37afdea700ba")
set(WNTRNLUI_VERSION
    ${WNTRNLUI_MAJOR_VERSION}.${WNTRNLUI_MINOR_VERSION}${WNTRNLUI_MICRO_VERSION})

list(APPEND CMAKE_MODULE_PATH "cmake/modules")


include(WintermuteDefaults)
include(WintermuteMacros)

find_package(WntrVisual 0.01 REQUIRED)
find_package(Wintermute 0.01 REQUIRED)
find_package(Qt4 COMPONENTS
     QtGui
     QtDBus
     QtCore
     REQUIRED)

file(GLOB WNTRNLUI_SHPP "src/*.hpp")
file(GLOB WNTRNLUI_SCPP "src/*.cpp")
file(GLOB WNTRNLUI_UIC "ui/*.ui")
file(GLOB WNTRNLUI_QRC "ui/*.qrc")
file(GLOB CMAKE_MODULES "${PROJECT_SOURCE_DIR}/cmake/Modules/*.cmake")

## Qt4
QT4_WRAP_CPP(WNTRNLUI_SCPP "${WNTRNLUI_SHPP}")
include("${QT_USE_FILE}")

## Main variables
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "Wintermute Natural User Interface")
set(WNTRNLUI_LIB_DIR "${WINTER_PLUGIN_INSTALL_DIR}")
set(WNTRNLUI_INCLUDE_DIR "${WINTER_PLUGIN_INCLUDE_INSTALL_DIR}/gui")
set(WNTRNLUI_INCLUDE_DIRS "${WINTERMUTE_INCLUDE_DIRS}"
    ${WNTRVISUAL_INCLUDE_DIRS}
    ${Qt_INCLUDE_DIRS})
set(WNTRNLUI_LIBRARIES ${QT_LIBRARIES} ${WINTERMUTE_LIBRARIES} ${WNTRVISUAL_LIBRARIES})
set(WNTRNLUI_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/WntrNlui" CACHE PATH "CMake directory.")


include_directories(${WNTRNLUI_INCLUDE_DIRS})

## Targets
add_library(nlui-wintermute SHARED ${WNTRNLUI_SCPP})

set_target_properties(nlui-wintermute PROPERTIES
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION})

target_link_libraries(nlui-wintermute ${WNTRNLUI_LIBRARIES})

## Configs
configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
    "${PROJECT_SOURCE_DIR}/src/config.hpp")

## Installs
install(TARGETS nlui-wintermute
    EXPORT WntrNluiLibraryDepends
    LIBRARY DESTINATION "${WNTRNLUI_LIB_DIR}"
    RENAME WNTRNLUI_UUID)

install(EXPORT WntrNluiLibraryDepends
    DESTINATION "${WNTRNLUI_CMAKE_DIR}")

install(FILES ${WNTRNLUI_SHPP}
    DESTINATION "${WNTRNLUI_INCLUDE_DIR}")

install(FILES ${CMAKE_MODULES}
    DESTINATION "${CMAKE_MODULE_DIR}")

wntrplugin_add_data("nlui-wintermute")