project(Wintermute)
set(PROJECT_LABEL "Wintermute Core")
set(PROJECT_LOGO "${PROJECT_SOURCE_DIR}/WintermuteLogo.png")
cmake_minimum_required(VERSION 2.8)
set(WINTERMUTE_MAJOR_VERSION 0)
set(WINTERMUTE_MINOR_VERSION 1)
set(WINTERMUTE_VERSION "${WINTERMUTE_MAJOR_VERSION}.${WINTERMUTE_MINOR_VERSION}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## Modules
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

## Wintermute SDK
include(WintermuteDefaults)
include(WintermuteMacros)

## Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE ${WINTER_BUILD_TYPE})
endif()

## Install prefix
set(CMAKE_INSTALL_PREFIX "/usr")

## Packages
find_package(PackageKitQt2 REQUIRED)
find_package(Boost COMPONENTS
    system
    program_options
    REQUIRED)
find_package(Qt4 COMPONENTS
    QtCore
    QtGui
    QtDBus
    REQUIRED)

include("${QT_USE_FILE}")

## Doxygen
enable_doxygen()

## Files
file(GLOB WINTER_CMAKE_MODULES "${PROJECT_SOURCE_DIR}/cmake/Modules/*.cmake")

## Main variables
set(VS_KEYWORD Qt4VSv1.0)
set(VS_SCC_PROJECTNAME "Wintermute Core")
set(WINTERMUTE_INCLUDE_DIR "${WINTER_INCLUDE_INSTALL_DIR}")
set(WINTERMUTE_INCLUDE_DIRS "${WINTERMUTE_INCLUDE_DIR}"
    ${Boost_INCLUDE_DIRS}
    ${QT_INCLUDE_DIRS}
    ${WINTER_PLUGIN_INCLUDE_INSTALL_DIR})
set(WINTERMUTE_LIBRARIES
    ${Boost_LIBRARIES}
    ${QT_LIBRARIES})
set(WINTERMUTE_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/Wintermute" CACHE PATH "CMake directory.")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O1")

## Include directories
include_directories(${WINTERMUTE_INCLUDE_DIRS})
include_directories(${PROJECT_BINARY_DIR})

## Configs
configure_file("${PROJECT_SOURCE_DIR}/cmake/WintermuteConfig.cmake.in"
               "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfig.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/cmake/WintermuteConfigVersion.cmake.in"
               "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfigVersion.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_BINARY_DIR}/src/config.hpp" @ONLY)

## D-Bus configuration
dbus_add_activation_service(services/org.thesii.Wintermute.service.in)

add_subdirectory(src)
add_subdirectory(cmake/Modules)

## Installs
install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfig.cmake"
              "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfigVersion.cmake"
        DESTINATION "${WINTERMUTE_CMAKE_DIR}")

## @todo Add it to a system wide location.
install(FILES "${PROJECT_SOURCE_DIR}/Wintermute.conf"
        DESTINATION "~/.config/Synthetic Intellect Institute")

