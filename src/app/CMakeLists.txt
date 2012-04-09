cmake_minimum_required(VERSION 2.8)
project(WintermuteApplication)

include(WintermuteDefaults)
include(WintermuteMacros)

file(GLOB WINTERMUTE_SOURCES "*.cpp" "*.cxx")
file(GLOB WINTERMUTE_HEADERS "*.hpp" "*.hxx")

set(WINTERMUTE_APP_LIBRARIES ${WINTERMUTE_LIBRARIES})
set(WINTERMUTE_BUILD_INCLUDE_DIRS ${WINTERMUTE_INCLUDE_DIRS}
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}")

if (WITH_GUI)
    list(APPEND WINTERMUTE_APP_LIBRARIES "${QT_QTGUI_LIBRARY}")
else (WITH_GUI)
    list(REMOVE_ITEM WINTERMUTE_APP_LIBRARIES "${QT_QTGUI_LIBRARY}")
endif(WITH_GUI)

include_directories(${WINTERMUTE_BUILD_INCLUDE_DIRS})

## Configures
configure_file("config.hpp.in" "config.hpp" @ONLY)

QT4_AUTOMOC(${WINTERMUTE_SOURCES})

## Targets
add_executable(wintermute ${WINTERMUTE_SOURCES})
target_link_libraries(wintermute ${WINTERMUTE_APP_LIBRARIES})
set_target_properties(wintermute PROPERTIES
    COMPILE_DEFINITIONS "${WINTERMUTE_COMPILE_DEFINITIONS}"
    COMPILE_FLAGS ${WINTERMUTE_COMPILE_FLAGS})

## Installs
install(TARGETS wintermute
    RUNTIME DESTINATION "${WINTER_BIN_INSTALL_DIR}"
    COMPONENT bin)

install(FILES ${WINTERMUTE_HEADERS}
    DESTINATION "${WINTER_INCLUDE_INSTALL_DIR}"
    COMPONENT dev)