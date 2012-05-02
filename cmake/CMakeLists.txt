cmake_minimum_required(VERSION 2.8)

set(WINTERMUTE_BASE_APPLICATION "yoo")
add_subdirectory(Modules)

## Installs
install(FILES "${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
              "${PROJECT_SOURCE_DIR}/cmake/pluginspec.in"
        DESTINATION "${WINTER_CMAKE_DIR}")