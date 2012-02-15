cmake_minimum_required(VERSION 2.8)

file(GLOB WINTER_CMAKE_MODULES "*.cmake")

install(FILES ${WINTER_CMAKE_MODULES}
    DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake-2.8/Modules"
    COMPONENT dev)
