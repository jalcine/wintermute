project(WntrDataLibrary)
cmake_minimum_required(VERSION 2.8)

## Files
file(GLOB WNTRDATA_HDRS "*.hpp")
file(GLOB WNTRDATA_SRCS "*.cpp")
file(GLOB WNTRDATA_DBUS_HDRS "dbus/*.hpp")
file(GLOB WNTRDATA_DBUS_SRCS "dbus/*.cpp")
file(GLOB WNTRDATA_LEXICAL_HDRS "lexical/*.hpp")
file(GLOB WNTRDATA_LEXICAL_SRCS "lexical/*.cpp")
file(GLOB WNTRDATA_ONTOLOGY_HDRS "ontology/*.hpp")
file(GLOB WNTRDATA_ONTOLOGY_SRCS "ontology/*.cpp")
file(GLOB WNTRDATA_SYNTAX_HDRS "syntax/*.hpp")
file(GLOB WNTRDATA_SYNTAX_SRCS "syntax/*.cpp")

set(WNTRDATA_HEADERS ${WNTRDATA_HDRS}
                     ${WNTRDATA_DBUS_HDRS}
                     ${WNTRDATA_LEXICAL_HDRS}
                     ${WNTRDATA_ONTOLOGY_HDRS}
                     ${WNTRDATA_SYNTAX_HDRS})
set(WNTRDATA_SOURCES ${WNTRDATA_SRCS}
                     ${WNTRDATA_DBUS_SRCS}
                     ${WNTRDATA_LEXICAL_SRCS}
                     ${WNTRDATA_ONTOLOGY_SRCS}
                     ${WNTRDATA_SYNTAX_SRCS})

QT4_AUTOMOC(${WNTRDATA_SOURCES})

set(WNTRDATA_SOURCE_DIRECTORIES "dbus"
    "syntax"
    "lexical"
    "ontology")

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
        "${PROJECT_SOURCE_DIR}/config.hpp" @ONLY)

foreach(WNTRDATA_SOURCE_DIRECTORY ${WNTRDATA_SOURCE_DIRECTORIES})
    add_subdirectory("${WNTRDATA_SOURCE_DIRECTORY}")
endforeach(WNTRDATA_SOURCE_DIRECTORY)

include_directories(${WNTRDATA_INCLUDE_DIRS}
                    "${PROJECT_SOURCE_DIR}"
                    "${PROJECT_BINARY_DIR}")

## Targets
add_library(wapi-data SHARED ${WNTRDATA_SOURCES})

set_target_properties(wapi-data PROPERTIES
    COMPILE_DEFINITIONS "${WNTRDATA_COMPILE_DEFINTIONS}"
    COMPILE_FLAGS "${WNTRDATA_COMPILE_FLAGS}"
    DEFINE_SYMBOL "WNTR_EXPORT_SYMBOLS"
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION})

target_link_libraries(wapi-data ${WNTRDATA_LIBRARIES})

## Installs
install(TARGETS wapi-data
    EXPORT WntrDataLibraryDepends
    LIBRARY DESTINATION "${WINTER_PLUGIN_INSTALL_DIR}")

install(EXPORT WntrDataLibraryDepends
    DESTINATION "${WNTRDATA_CMAKE_DIR}")

install(FILES ${WNTRDATA_HEADERS}
    DESTINATION "${WNTRDATA_INCLUDE_DIR}"
    COMPONENT dev)
