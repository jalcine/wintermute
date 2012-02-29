cmake_minimum_required(VERSION 2.8)

add_subdirectory(services)

install(FILES "${PROJECT_SOURCE_DIR}/Wintermute.conf"
        DESTINATION "~/.config/Synthetic Intellect Institute")