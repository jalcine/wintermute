project(WntrDataLibrary)
cmake_minimum_required(VERSION 2.8)

## Files
file(GLOB_RECURSE WNTRDATA_SOURCES "*.cpp" "*.cxx")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/lexical")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/dbus")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/syntax")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/ontology")

QT4_AUTOMOC(${WNTRDATA_SOURCES})

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
        "${PROJECT_SOURCE_DIR}/config.hpp" @ONLY)

include_directories(${WNTRDATA_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_BINARY_DIR}")

## Targets
add_library(wapi-data SHARED ${WNTRDATA_SOURCES})

set_target_properties(wapi-data PROPERTIES
    COMPILE_DEFINITIONS "${WNTRDATA_COMPILE_DEFINTIONS}"
    COMPILE_FLAGS "${WNTRDATA_COMPILE_FLAGS}"
    DEFINE_SYMBOL "WNTR_EXPORT_SYMBOLS"
    VERSION ${WNTRDATA_VERSION}
    SOVERSION ${WNTRDATA_SOVERSION})

target_link_libraries(wapi-data ${WNTRDATA_LIBRARIES})

## Installs
install(TARGETS wapi-data
    EXPORT WntrDataLibraryDepends
    LIBRARY DESTINATION "${WINTER_PLUGIN_INSTALL_DIR}")

install(EXPORT WntrDataLibraryDepends
    DESTINATION "${WNTRDATA_CMAKE_DIR}")

install(DIRECTORY "${PROJECT_SOURCE_DIR}/"
        DESTINATION "${WNTRDATA_INCLUDE_DIR}"
        COMPONENT dev
        FILES_MATCHING
            PATTERN "*.hpp"
            PATTERN "*.hxx"
            PATTERN "*_p.hxx" EXCLUDE
        )
