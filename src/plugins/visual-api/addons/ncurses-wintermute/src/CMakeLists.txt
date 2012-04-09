cmake_minimum_required(VERSION 2.8)

file(GLOB WNCURSES_SHPP "*.hpp")
file(GLOB WNCURSES_SCPP "*.cpp")

## Qt4
QT4_WRAP_CPP(WNCURSES_SCPP "${WNCURSES_SHPP}")

include_directories(${WNCURSES_INCLUDE_DIRS})

## Targets
add_library(ncurses-wintermute SHARED ${WNCURSES_SCPP})

set_target_properties(ncurses-wintermute PROPERTIES
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION})

target_link_libraries(ncurses-wintermute ${WNCURSES_LIBRARIES})

configure_file("config.hpp.in" "config.hpp" @ONLY)

## Installs
install(TARGETS ncurses-wintermute
    EXPORT NCursesWintermuteLibraryDepends
    LIBRARY DESTINATION "${WNCURSES_LIB_DIR}"
    RENAME WNCURSES_UUID)

install(EXPORT NCursesWintermuteLibraryDepends
    DESTINATION "${WNCURSES_CMAKE_DIR}")

install(FILES ${WNCURSES_SHPP}
    DESTINATION "${WNCURSES_INCLUDE_DIR}")
