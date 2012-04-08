cmake_minimum_required(VERSION 2.8)

add_subdirectory(app)

if (WITH_PLUGINS)
    message(STATUS "Plug-ins are being built; activating default set.")
    if(NOT DEFINED BUILD_DATA_API)
        set(BUILD_DATA_API ON)
    endif(NOT DEFINED BUILD_DATA_API)

    if(NOT DEFINED BUILD_NETWORK_API)
        set(BUILD_NETWORK_API ON)
    endif(NOT DEFINED BUILD_NETWORK_API)

    if(NOT DEFINED BUILD_LINGUISTICS_API)
        set(BUILD_LINGUISTICS_API ON)
    endif(NOT DEFINED BUILD_LINGUISTICS_API)

    add_subdirectory(plugins)
endif(WITH_PLUGINS)
