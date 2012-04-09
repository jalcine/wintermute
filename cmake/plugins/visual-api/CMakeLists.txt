cmake_minimum_required(VERSION 2.8)

configure_file("WntrVisualConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrVisualConfig.cmake" @ONLY)

configure_file("WntrVisualConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrVisualConfigVersion.cmake" @ONLY)

install(FILES
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrVisualConfig.cmake"
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrVisualConfigVersion.cmake"
    DESTINATION "${WNTRVISUAL_CMAKE_DIR}")

install(FILES ${WNTRVISUAL_CMAKE_MODULES}
    DESTINATION "${WNTRVISUAL_CMAKE_DIR}")
