cmake_minimum_required(VERSION 2.8)

add_subdirectory(app)

if (WITH_PLUGINS)
    add_subdirectory(plugins)
endif(WITH_PLUGINS)
