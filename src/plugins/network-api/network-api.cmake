project(WntrNtwkLib)
cmake_minimum_required(VERSION 2.8)

set(WNTRNTWK_BUILD_INCLUDE_DIRS ${WNTRNTWK_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_BINARY_DIR}")

include_directories(${WNTRNTWK_BUILD_INCLUDE_DIRS})

## Files
file(GLOB WNTRNTWK_SHPP "*.hpp")
file(GLOB WNTRNTWK_SCPP "*.cpp")
QT4_AUTOMOC(${WNTRNTWK_SCPP})

configure_file("config.hpp.in" "config.hpp" @ONLY)

## Targets
add_library(wapi-network SHARED
    ${WNTRNTWK_SCPP})

set_target_properties(wapi-network PROPERTIES
    COMPILE_DEFINITIONS "${WNTRNTWK_COMPILE_DEFINTIONS}"
    COMPILE_FLAGS "${WNTRNTWK_COMPILE_FLAGS}"
    DEFINE_SYMBOL "WNTRNTWK_EXPORT"
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION})

target_link_libraries(wapi-network
    ${WNTRNTWK_LIBRARIES})

## Installs
install(TARGETS wapi-network
    EXPORT WntrNtwkLibraryDepends
    LIBRARY DESTINATION "${WNTRNTWK_LIB_DIR}")

install(EXPORT WntrNtwkLibraryDepends
    DESTINATION "${WNTRNTWK_CMAKE_DIR}")

install(FILES ${WNTRNTWK_SHPP}
    DESTINATION "${WNTRNTWK_INCLUDE_DIR}")

