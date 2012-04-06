cmake_minimum_required(VERSION 2.8)

file(GLOB CMAKE_MODULES "${PROJECT_SOURCE_DIR}/cmake/Modules/*.cmake")

configure_file("WntrNtwkConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrNtwkConfig.cmake" @ONLY)

configure_file("WntrNtwkConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrNtwkConfigVersion.cmake" @ONLY)

install(FILES
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrNtwkConfig.cmake"
    "${PROJECT_BINARY_DIR}/InstallFiles/WntrNtwkConfigVersion.cmake"
    DESTINATION "${WNTRNTWK_CMAKE_DIR}")

install(FILES ${CMAKE_MODULES}
    DESTINATION "${CMAKE_MODULE_DIR}")