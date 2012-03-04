cmake_minimum_required(VERSION 2.8)

add_subdirectory(services)
add_subdirectory(res)

install(FILES "Wintermute.conf"
        DESTINATION "~/.config/Synthetic Intellect Institute")