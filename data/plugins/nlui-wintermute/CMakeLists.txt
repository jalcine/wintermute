cmake_minimum_required(VERSION 2.8)

configure_file("spec.in"
               "${PROJECT_BINARY_DIR}/InstallFiles/${WNTRNLUI_UUID}.spec" @ONLY)

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/${WNTRNLUI_UUID}.spec"
    DESTINATION "${WINTER_PLUGINSPEC_INSTALL_DIR}")

