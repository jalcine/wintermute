cmake_minimum_required(VERSION 2.8)

set(WNCURSES_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/ncurses-wintermute" CACHE PATH "CMake directory.")

file(GLOB CMAKE_MODULES "${PROJECT_SOURCE_DIR}/cmake/Modules/*.cmake")

configure_file("NCursesWintermuteConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/NCursesWintermuteConfig.cmake" @ONLY)

configure_file("NCursesWintermuteConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/NCursesWintermuteConfigVersion.cmake" @ONLY)

install(FILES
    "${PROJECT_BINARY_DIR}/InstallFiles/NCursesWintermuteConfig.cmake"
    "${PROJECT_BINARY_DIR}/InstallFiles/NCursesWintermuteConfigVersion.cmake"
    DESTINATION "${WNCURSES_CMAKE_DIR}")

install(FILES ${CMAKE_MODULES}
    DESTINATION "${CMAKE_MODULE_DIR}")
