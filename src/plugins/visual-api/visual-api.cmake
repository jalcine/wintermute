cmake_minimum_required(VERSION 2.8)
project(WntrVisualLibrary)
file(GLOB WNTRVISUAL_SHPP "*.hpp")
file(GLOB WNTRVISUAL_SCPP "*.cpp")

## Qt4
QT4_WRAP_CPP(WNTRVISUAL_SCPP "${WNTRVISUAL_SHPP}")
QT4_WRAP_UI(WNTRVISUAL_SCPP "${WNTRVISUAL_UI}")
QT4_ADD_RESOURCES(WNTRVISUAL_SQRC "${WNTRVISUAL_QRC}")

## Targets
include_directories(${WNTRVISUAL_INCLUDE_DIRS})

add_library(wapi-visual SHARED ${WNTRVISUAL_SCPP} ${WNTRVISUAL_SQRC})

set_target_properties(wapi-visual PROPERTIES
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION})

target_link_libraries(wapi-visual ${WNTRVISUAL_LIBRARIES})

configure_file("config.hpp.in" "config.hpp" @ONLY)

install(TARGETS wapi-visual
    EXPORT WntrVisualLibraryDepends
    LIBRARY DESTINATION "${WNTRVISUAL_LIB_DIR}"
    RENAME WNTRVISUAL_UUID)

install(EXPORT WntrVisualLibraryDepends
    DESTINATION "${WNTRVISUAL_CMAKE_DIR}")

install(FILES ${WNTRVISUAL_SHPP}
    DESTINATION "${WNTRVISUAL_INCLUDE_DIR}")