cmake_minimum_required(VERSION 2.8)
project(WintermuteApplication)

include(WintermuteDefaults)
include(WintermuteMacros)

configure_file("config.hpp.in"
               "config.hpp" @ONLY)

file(GLOB WINTERMUTE_SOURCES "*.cpp")
file(GLOB WINTERMUTE_HEADERS "*.hpp")
QT4_WRAP_CPP(WINTERMUTE_SOURCES "${WINTERMUTE_HEADERS}")

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
