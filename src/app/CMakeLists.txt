cmake_minimum_required(VERSION 2.8)
project(WintermuteApplication)

include(WintermuteDefaults)
include(WintermuteMacros)

file(GLOB WINTERMUTE_SOURCES "*.cpp")
file(GLOB WINTERMUTE_HEADERS "*.hpp")

if (WITH_GUI_SUPPORT)
    list(APPEND WINTERMUTE_LIBRARIES ${QT_QTGUI_LIBRARY})
    message(STATUS "Enabled GUI support in Wintermute.")
else (WITH_GUI_SUPPORT)
    list(REMOVE_ITEM WINTERMUTE_LIBRARIES ${QT_QTGUI_LIBRARY})
    message(STATUS "Disabled GUI support in Wintermute.")
endif (WITH_GUI_SUPPORT)

include_directories(${WINTERMUTE_BUILD_INCLUDE_DIRS})

## Configures
configure_file("config.hpp.in" "config.hpp" @ONLY)

QT4_AUTOMOC(${WINTERMUTE_SOURCES})

## Targets
add_executable(wintermute ${WINTERMUTE_SOURCES})
target_link_libraries(wintermute ${WINTERMUTE_LIBRARIES})
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
