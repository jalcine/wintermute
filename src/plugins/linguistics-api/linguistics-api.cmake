cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB WNTRLING_SRCS "*.cpp")
file(GLOB WNTRLING_HDRS "*.hpp")
file(GLOB WNTRLING_DBUS_SRCS "dbus/*.cpp")
file(GLOB WNTRLING_DBUS_HDRS "dbus/*.hpp")
file(GLOB WNTRLING_PARSER_SRCS "parser/*.cpp")
file(GLOB WNTRLING_PARSER_HDRS "parser/*.hpp")
file(GLOB WNTRLING_PARSER_PRIVSRCS "parser/*.cxx")
file(GLOB WNTRLING_PARSER_PRIVHDRS "parser/*.hxx")
file(GLOB WNTRLING_SYNTAX_SRCS "syntax/*.cpp")
file(GLOB WNTRLING_SYNTAX_HDRS "syntax/*.hpp")

set(WNTRLING_SOURCES ${WNTRLING_SRCS}
                     ${WNTRLING_DBUS_SRCS}
                     ${WNTRLING_PARSER_SRCS}
                     ${WNTRLING_PARSER_PRIVSRCS}
                     ${WNTRLING_SYNTAX_SRCS})
set(WNTRLING_HEADERS ${WNTRLING_HDRS}
                     ${WNTRLING_DBUS_HDRS}
                     ${WNTRLING_PARSER_HDRS}
                     ${WNTRLING_SYNTAX_HDRS})

QT4_WRAP_CPP(WNTRLING_SOURCES ${WNTRLING_HEADERS})

set(WNTRLING_SOURCE_DIRECTORIES "dbus"
    "syntax"
    "parser")

foreach(WNTRLING_SOURCE_DIRECTORY ${WNTRLING_SOURCE_DIRECTORIES})
    add_subdirectory("${WNTRLING_SOURCE_DIRECTORY}")
endforeach(WNTRLING_SOURCE_DIRECTORY)

configure_file("config.hpp.in" "config.hpp" @ONLY)

include_directories("${PROJECT_SOURCE_DIR}/src")

## Library
add_library(wapi-linguistics SHARED
    ${WNTRLING_SOURCES})

set_target_properties(wapi-linguistics PROPERTIES
    COMPILE_DEFINITIONS "${WNTRLING_COMPILE_DEFINTIONS}"
    COMPILE_FLAGS "${WNTRLING_COMPILE_FLAGS}"
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION})

target_link_libraries(wapi-linguistics ${WNTRLING_LIBRARIES})

## Installs
install(TARGETS wapi-linguistics
    EXPORT WntrLingLibraryDepends
    LIBRARY DESTINATION "${WNTRLING_LIB_DIR}")

#install(EXPORT WntrLingLibraryDepends
#    DESTINATION "${WNTRLING_CMAKE_DIR}")

install(FILES ${WNTRLING_HEADERS}
    DESTINATION "${WNTRLING_INCLUDE_DIR}"
    COMPONENT dev)