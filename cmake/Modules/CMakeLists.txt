cmake_minimum_required(VERSION 2.8)

include(WintermuteDefaults)

file(GLOB WINTER_CMAKE_MODULES "*.cmake")

configure_file("${CMAKE_SOURCE_DIR}/cmake/Modules/FindWintermute.cmake.in"
               "${CMAKE_BINARY_DIR}/InstallFiles/FindWintermute.cmake" @ONLY)

install(FILES ${WINTER_CMAKE_MODULES}
              "${CMAKE_BINARY_DIR}/InstallFiles/FindWintermute.cmake"
        DESTINATION "${CMAKE_RECENT_MODULE_DIR}"
        COMPONENT dev)