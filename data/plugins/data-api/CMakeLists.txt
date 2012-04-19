cmake_minimum_required(VERSION 2.8)

add_subdirectory(dbus)

configure_file("spec.in"
               "${PROJECT_BINARY_DIR}/InstallFiles/${WNTRDATA_UUID}.spec" @ONLY)

## Installs
install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/${WNTRDATA_UUID}.spec"
        DESTINATION "${WINTER_PLUGIN_SPEC_INSTALL_DIR}")

install(DIRECTORY ${WNTRDATA_LING_DIR}
                  ${WNTRDATA_ONTO_DIR}
        DESTINATION "${WNTRDATA_DATA_DIR}")
