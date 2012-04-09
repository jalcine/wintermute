cmake_minimum_required(VERSION 2.8)

configure_file("WntrLingConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrLingConfig.cmake" @ONLY)

configure_file("WntrLingConfigVersion.cmake.in"
        "${PROJECT_BINARY_DIR}/InstallFiles/WntrLingConfigVersion.cmake" @ONLY)

install(FILES
        "${PROJECT_BINARY_DIR}/InstallFiles/WntrLingConfig.cmake"
        "${PROJECT_BINARY_DIR}/InstallFiles/WntrLingConfigVersion.cmake"
        DESTINATION "${WNTRLING_CMAKE_DIR}")
