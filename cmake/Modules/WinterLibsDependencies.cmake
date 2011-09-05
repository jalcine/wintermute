if (NOT WINTER_INSTALL_DIR)
    set(WINTER_INSTALL_DIR /usr)
endif()

set(WINTER_BIN_INSTALL_DIR /usr/bin)
set(WINTER_LIB_INSTALL_DIR /usr/lib)
set(WINTER_PLUGIN_INSTALL_DIR "${WINTER_LIB_INSTALL_DIR}/wntr")
set(WINTER_INCLUDE_INSTALL_DIR /usr/include)
set(WINTER_DATA_INSTALL_DIR /usr/share/wintermute)
set(WINTER_PLUGINSPEC_INSTALL_DIR "${WINTER_DATA_INSTALL_DIR}/plugins")

## Our CMake default Module-dir
set(CMAKE_MODULE_DIR /usr/share/cmake-2.8/Modules)
