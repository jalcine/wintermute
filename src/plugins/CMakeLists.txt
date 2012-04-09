cmake_minimum_required(VERSION 2.8)

include(CMakeDependentOption)

macro(include_api _api __api_name)
    set(__api ${_api})
    set(__api_name ${__api_name})
    set(__api_path "${CMAKE_BINARY_DIR}/src/plugins/${__api}/InstallFiles")
    set(_include_path "${CMAKE_BINARY_DIR}/src/plugins/${__api_name}.include")
    file(READ "include.cmake.in" _include_template)
    string(CONFIGURE ${_include_template} _include_template @ONLY)
    file(WRITE ${_include_path} ${_include_template})
    include(${_include_path})
endmacro(include_api _api __api_name)

macro(wntrplugin_add_data _api)
    add_subdirectory("${CMAKE_SOURCE_DIR}/cmake/plugins/${_api}" "${CMAKE_BINARY_DIR}/cmake/plugins/${_api}")
    add_subdirectory("${CMAKE_SOURCE_DIR}/data/plugins/${_api}" "${CMAKE_BINARY_DIR}/data/plugins/${_api}")
endmacro(wntrplugin_add_data _api)

set(Wintermute_DIR "${CMAKE_BINARY_DIR}/InstallFiles")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

option(BUILD_DATA_API         "Build the Data API."         ON)
option(BUILD_NETWORK_API      "Build the Network API."      ON)
option(BUILD_LINGUISTICS_API  "Build the Linguistics API."  ON)
cmake_dependent_option(BUILD_VISUAL_API "Build the Visual API." ON
                       "WITH_GUI" OFF)

if (BUILD_DATA_API)
    include_api("data-api" "WntrData")
endif(BUILD_DATA_API)

if (BUILD_NETWORK_API)
    include_api("network-api" "WntrNtwk")
endif(BUILD_NETWORK_API)

if (BUILD_LINGUISTICS_API)
    set(BUILD_DATA_API ON)
    include_api("data-api" "WntrData")
    include_api("linguistics-api" "WntrLing")
endif(BUILD_LINGUISTICS_API)

if (BUILD_VISUAL_API)
    include_api("visual-api" "WntrVisual")
endif(BUILD_VISUAL_API)