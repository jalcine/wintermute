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

set(WINTER_BIN_INSTALL_DIR "${WINTER_INSTALL_DIR}/bin" CACHE PATH "Binary installation path.")
set(WINTER_LIB_INSTALL_DIR "${WINTER_INSTALL_DIR}/lib" CACHE PATH "Library installation path.")
set(WINTER_INCLUDE_INSTALL_DIR "${WINTER_INSTALL_DIR}/include/wntr" CACHE PATH "Header installation path.")
set(WINTER_DATA_INSTALL_DIR "${WINTER_INSTALL_DIR}/share/wintermute" CACHE PATH "Data installation path.")
set(WINTER_PLUGIN_INSTALL_DIR "${WINTER_LIB_INSTALL_DIR}/wntr" CACHE PATH "Plugin installation path.")
set(WINTER_PLUGIN_INCLUDE_INSTALL_DIR "${WINTER_INCLUDE_INSTALL_DIR}/plugins" CACHE PATH "Plugin header installation path.")
set(WINTER_PLUGIN_DATA_INSTALL_DIR "${WINTER_DATA_INSTALL_DIR}/plugins" CACHE PATH "Plugin data installation path.")
set(WINTER_PLUGINSPEC_INSTALL_DIR "${WINTER_DATA_INSTALL_DIR}/specs" CACHE PATH "Plugin specification installation path.")

set(WINTERMUTE_IS_PLUGIN OFF CACHE STRING "The status of this project (is it a plug-in?)")
set(WINTERMUTE_LOCALE "en" CACHE STRING "The standard locale for Wintermute to use.")

## Standard CMake information.
list(APPEND CMAKE_MODULE_DIR "${WINTER_INSTALL_DIR}/share/cmake-2.8/Modules"
                             "${WINTER_INSTALL_DIR}/share/cmake/Modules")

add_definitions("-Wall")