cmake_minimum_required(VERSION 2.8)

## Configs
configure_file("spec.in"
               "${PROJECT_BINARY_DIR}/InstallFiles/${WNCURSES_UUID}.spec" @ONLY)

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/${WNCURSES_UUID}.spec"
    DESTINATION "${WINTER_PLUGINSPEC_INSTALL_DIR}")

