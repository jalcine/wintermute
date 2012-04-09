cmake_minimum_required(VERSION 2.8)

file(GLOB WNTRDATA_CMAKE_MODULES "/Modules/*.cmake")

configure_file("WntrDataConfig.cmake.in"
        "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfig.cmake" @ONLY)

configure_file("WntrDataConfigVersion.cmake.in"
        "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfigVersion.cmake" @ONLY)

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfig.cmake"
              "${PROJECT_BINARY_DIR}/InstallFiles/WntrDataConfigVersion.cmake"
        DESTINATION "${WNTRDATA_CMAKE_DIR}")

install(FILES ${WNTRDATA_CMAKE_MODULES}
        DESTINATION "${WNTRDATA_CMAKE_DIR}")
