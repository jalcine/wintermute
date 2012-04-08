cmake_minimum_required(VERSION 2.8)

option(BUILD_DATA_API          "Build the Data API."        OFF)
option(BUILD_NETWORK_API       "Build the Network API."     OFF)
option(BUILD_LINGUISTICS_API   "Build the Linguistics API." OFF)
option(BUILD_VISUAL_API        "Build the Visual API."      OFF)

set(Wintermute_DIR "${CMAKE_BINARY_DIR}/InstallFiles")
if (BUILD_DATA_API)
    add_subdirectory(data-api)
    set(WntrData_DIR "${CMAKE_BINARY_DIR}/src/plugins/data-api/InstallFiles")
endif(BUILD_DATA_API)

if (BUILD_NETWORK_API)
    add_subdirectory(ntwk-api)
    set(WntrNtwk_DIR "${CMAKE_BINARY_DIR}/src/plugins/ntwk-api/InstallFiles")
endif(BUILD_NETWORK_API)

if (BUILD_LINGUISTICS_API)
    add_subdirectory(ling-api)
    set(WntrLing_DIR "${CMAKE_BINARY_DIR}/src/plugins/ling-api/InstallFiles")
endif(BUILD_LINGUISTICS_API)

if (BUILD_VISUAL_API)
    add_subdirectory(visual-api)
endif(BUILD_VISUAL_API)