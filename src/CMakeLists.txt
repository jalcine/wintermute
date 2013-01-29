cmake_minimum_required(VERSION 2.8)
project(WintermuteApplication)

## MODULE INCLUDES
## ===============
include(FindWintermute)

## SOURCE CODE MANIPULATION
## ========================
## Generate the global.hpp file for Wintermute.
configure_file("${PROJECT_SOURCE_DIR}/global.hpp.in"
               "${PROJECT_SOURCE_DIR}/global.hpp" @ONLY)

## Grab all of the files to be compiled.
file(GLOB WINTERMUTE_SOURCES "*.cpp")

## BUILD VARIABLES
## ===============
set(WINTERMUTE_BUILD_INCLUDE_DIRS "${WINTERMUTE_INCLUDE_DIRS}"
    ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})

## Make sure the build environment can't find silly files.
list(REMOVE_ITEM WINTERMUTE_BUILD_INCLUDE_DIRS ${WINTERMUTE_INCLUDE_DIR})
list(REMOVE_ITEM WINTERMUTE_BUILD_INCLUDE_DIRS ${WINTER_PLUGIN_INCLUDE_INSTALL_DIR})

## Include the build directories.
include_directories(${WINTERMUTE_BUILD_INCLUDE_DIRS})

## Wrap up the sources and generate documentation.
wntr_automoc("${WINTERMUTE_SOURCES}")
wntr_make_docs(PATH "${PROJECT_SOURCE_DIR}")

## TARGETS
## =======
## = Define and describe the Wintermute executable.
add_executable(wintermute ${WINTERMUTE_SOURCES})
target_link_libraries(wintermute ${WINTERMUTE_LIBRARIES})
set_target_properties(wintermute PROPERTIES
    DEFINE_SYMBOL "WINTER_EXPORT"
    VERSION "${WINTERMUTE_VERSION}"
    COMPILE_DEFINITIONS "${WINTERMUTE_COMPILE_DEFINITIONS}"
    COMPILE_FLAGS "${WINTERMUTE_COMPILE_FLAGS}")

## INSTALLATIONS
## =============
## = Installs the artificats generated.
install(TARGETS wintermute
        RUNTIME DESTINATION "${WINTER_BIN_INSTALL_DIR}"
        COMPONENT bin)
        
wntr_install_headers(HEADERS_PATH "${PROJECT_SOURCE_DIR}"
                     DESTINATION  "${WINTER_INCLUDE_INSTALL_DIR}")

configure_file("${CMAKE_SOURCE_DIR}/cmake/Modules/FindWintermute.cmake.in"
               "${CMAKE_BINARY_DIR}/InstallFiles/FindWintermute.cmake" @ONLY)
