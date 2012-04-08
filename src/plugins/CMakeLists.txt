cmake_minimum_required(VERSION 2.8)

include(CMakeDependentOption)

set(Wintermute_DIR "${CMAKE_BINARY_DIR}/InstallFiles")

option(BUILD_DATA_API         "Build the Data API."         ON)
option(BUILD_NETWORK_API      "Build the Network API."      ON)
option(BUILD_LINGUISTICS_API  "Build the Linguistics API."  ON)
cmake_dependent_option(BUILD_VISUAL_API "Build the Visual API." ON
                       "WITH_GUI;WITH_PLUGINS" OFF)

macro(enable_api _api _name)
    set(_path_i "${CMAKE_BINARY_DIR}/src/plugins/${_api}/InstallFiles")

    if (NOT EXISTS ${_path_i})
        add_subdirectory(${_api})
        set("${_name}_DIR" ${_path_i})
    endif (NOT EXISTS ${_path_i})
endmacro(enable_api _api)

if (BUILD_DATA_API)
    enable_api("data-api","WntrData")
endif(BUILD_DATA_API)

if (BUILD_NETWORK_API)
    enable_api("ntwk-api","WntrNtwk")
endif(BUILD_NETWORK_API)

if (BUILD_LINGUISTICS_API)
    set(BUILD_DATA_API ON)
    enable_api("data-api","WntrData")
    enable_api("ling-api","WntrLing")
endif(BUILD_LINGUISTICS_API)

if (BUILD_VISUAL_API)
    enable_api("visual-api","WntrVisual")
endif(BUILD_VISUAL_API)