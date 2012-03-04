## - Contains important variables required for building Wintermute.
##

## Useful options and variables
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

set(GENERIC_LIB_VERSION 0.0.1)
set(GENERIC_LIB_SOVERSION 0)
set(WINTER_NON_GENERIC_LIB_VERSION 0.0.1)
set(WINTER_NON_GENERIC_LIB_SOVERSION 1)

set(WINTER_BUILD_TYPE "RelWithDebInfo")
set(WINTER_INSTALL_DIR "/usr" CACHE PATH "Base installation path for Wintermute installation files.")
set(WINTER_CMAKE_DIR "${WINTER_INSTALL_DIR}/share/cmake/Wintermute" CACHE PATH "CMake directory for Wintermute.")
set(WINTER_BIN_INSTALL_DIR "${WINTER_INSTALL_DIR}/bin" CACHE PATH "Binary installation path.")
set(WINTER_LIB_INSTALL_DIR "${WINTER_INSTALL_DIR}/lib" CACHE PATH "Library installation path.")
set(WINTER_INCLUDE_INSTALL_DIR "${WINTER_INSTALL_DIR}/include/wintermute" CACHE PATH "Header installation path.")
set(WINTER_DATA_INSTALL_DIR "${WINTER_INSTALL_DIR}/share/wintermute" CACHE PATH "Data installation path.")
set(WINTER_PLUGIN_INSTALL_DIR "${WINTER_LIB_INSTALL_DIR}/wintermute" CACHE PATH "Plugin installation path.")
set(WINTER_PLUGIN_INCLUDE_INSTALL_DIR "${WINTER_INCLUDE_INSTALL_DIR}/plugins" CACHE PATH "Plugin header installation path.")
set(WINTER_PLUGIN_DATA_INSTALL_DIR "${WINTER_DATA_INSTALL_DIR}/plugins" CACHE PATH "Plugin data installation path.")
set(WINTER_PLUGINSPEC_INSTALL_DIR "${WINTER_DATA_INSTALL_DIR}/specs" CACHE PATH "Plugin specification installation path.")

set(WINTERMUTE_LOCALE "en" CACHE STRING "The standard locale for Wintermute to use.")

## Standard CMake information.
list(APPEND CMAKE_MODULE_DIR "${WINTER_INSTALL_DIR}/share/cmake-2.8/Modules"
                             "${WINTER_INSTALL_DIR}/share/cmake/Modules")

## Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE ${WINTER_BUILD_TYPE})
endif()

if (NOT _wntr_set_uninstall)
    set(_wntr_set_uninstall true)
    set(_prefix)

    if (PROJECT_LABEL STREQUAL "Wintermute Core")
        set(_prefix "${CMAKE_SOURCE_DIR}/cmake")
    else (PROJECT_LABEL STREQUAL "Wintermute Core")
        set(_prefix "${WINTER_CMAKE_DIR}")
    endif(PROJECT_LABEL STREQUAL "Wintermute Core")

    configure_file("${_prefix}/cmake_uninstall.cmake.in"
                   "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)
    add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")
endif (NOT _wntr_set_uninstall)