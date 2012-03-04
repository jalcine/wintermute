cmake_minimum_required(VERSION 2.8)

add_subdirectory(Modules)

configure_file("${PROJECT_SOURCE_DIR}/cmake/WintermuteConfig.cmake.in"
               "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfig.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/cmake/WintermuteConfigVersion.cmake.in"
               "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfigVersion.cmake" @ONLY)

## Installs
install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfig.cmake"
              "${PROJECT_BINARY_DIR}/InstallFiles/WintermuteConfigVersion.cmake"
              "${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
              "${PROJECT_SOURCE_DIR}/cmake/Doxyfile.in"
        DESTINATION "${WINTER_CMAKE_DIR}")

