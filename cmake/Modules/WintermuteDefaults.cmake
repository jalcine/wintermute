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

set(WINTERMUTE_COMPILE_DEFINITIONS )
set(WINTERMUTE_COMPILE_FLAGS "--std=c++0x -Wall -Wextra -Wabi -ggdb -Wctor-dtor-privacy -Wformat -Wunused")

## Standard CMake information.
list(APPEND CMAKE_MODULE_DIR "${WINTER_INSTALL_DIR}/share/cmake-2.8/Modules"
                             "${WINTER_INSTALL_DIR}/share/cmake/Modules")

## Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE ${WINTER_BUILD_TYPE})
endif()

## Set up uninstallation.
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

## Set up documentation.
if (NOT _wntr_set_docs)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        find_program(HAVE_DOT dot)
        if(HAVE_DOT)
            message(STATUS "Found 'dot' program, Doxygen will use it to generate graphs for documentation.")
            set(HAVE_DOT YES)
        else(HAVE_DOT)
            set(HAVE_DOT NO)
        endif(HAVE_DOT)

        set(_wntr_set_docs true)
        set(_prefix)
        set(_docfile_prefix)

        if (PROJECT_LABEL STREQUAL "Wintermute Core")
            set(_prefix "${CMAKE_SOURCE_DIR}/cmake")
            set(_docfile_prefix "${CMAKE_SOURCE_DIR}/data/res")
            set(_docfile_logo "${_docfile_prefix}/wintermute.png")
        else (PROJECT_LABEL STREQUAL "Wintermute Core")
            set(_prefix "${WINTER_CMAKE_DIR}")
            set(_docfile_prefix "${WINTER_CMAKE_DIR}/doc")
            set(_docfile_logo "${WINTER_CMAKE_DIR}/res/wintermute.png")
        endif(PROJECT_LABEL STREQUAL "Wintermute Core")

        configure_file("${_prefix}/Doxyfile.in"
                    "${CMAKE_BINARY_DIR}/Doxyfile" @ONLY)

        add_custom_target(docs
            ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "Generating API documentation with Doxygen...")
    endif(DOXYGEN_FOUND)

    macro(wntr_install_docs _path)
        install(DIRECTORY ${CMAKE_BINARY_DIR}/doc/html/
                DESTINATION ${_path})
    endmacro(wntr_install_docs _path)
endif (NOT _wntr_set_docs)