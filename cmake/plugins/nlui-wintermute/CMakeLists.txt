cmake_minimum_required(VERSION 2.8)

configure_file("WntrNluiConfig.cmake.in"
               "${PROJECT_BINARY_DIR}/InstallFiles/WntrNluiConfig.cmake" @ONLY)

configure_file("WntrNluiConfigVersion.cmake.in"
               "${PROJECT_BINARY_DIR}/InstallFiles/WntrNluiConfigVersion.cmake" @ONLY)

install(FILES
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrNluiConfig.cmake"
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrNluiConfigVersion.cmake"
    DESTINATION "${WNTRNLUI_CMAKE_DIR}")