cmake_minimum_required(VERSION 2.8)

add_subdirectory(app)
set(WINTER_PLUGINS_BUILDING ON)

if (WITH_PLUGINS)
    message(STATUS "Plug-ins are being built.")
    add_subdirectory(plugins)
endif(WITH_PLUGINS)
