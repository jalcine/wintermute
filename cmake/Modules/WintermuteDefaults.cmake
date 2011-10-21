## - Contains important variables required for building Wintermute.
##

## Useful options and variables
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)

set(GENERIC_LIB_VERSION 0.0.1)
set(GENERIC_LIB_SOVERSION 0)

set(WINTER_NON_GENERIC_LIB_VERSION 0.0.1)
set(WINTER_NON_GENERIC_LIB_SOVERSION 1)
set(WINTER_INSTALL_DIR "/usr" CACHE PATH "Base installation path for Wintermute installation files.")

if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "${WINTER_INSTALL_DIR}")
else()
    set(WINTER_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")
endif()

set(WINTER_BIN_INSTALL_DIR "${WINTER_INSTALL_DIR}/bin" CACHE PATH "Binary installation path.")
set(WINTER_LIB_INSTALL_DIR "${WINTER_INSTALL_DIR}/lib" CACHE PATH "Library installation path.")
set(WINTER_INCLUDE_INSTALL_DIR "${WINTER_INSTALL_DIR}/include/wntr" CACHE PATH "Header installation path.")
set(WINTER_DATA_INSTALL_DIR "${WINTER_INSTALL_DIR}/share/wintermute" CACHE PATH "Data installation path.")

set(WINTERMUTE_LOCALE "en" CACHE STRING "The standard locale for Wintermute to use.")

## Standard CMake information.
set(CMAKE_MODULE_DIR "${WINTER_INSTALL_DIR}/share/cmake-2.8/Modules")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/share/cmake-2.6/Modules")
