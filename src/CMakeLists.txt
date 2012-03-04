project(WintermuteApplication)

include(WintermuteDefaults)
include(WintermuteMacros)

set(WINTERMUTE_COMPILE_DEFINITIONS )
set(WINTERMUTE_COMPILE_FLAGS "--std=c++0x -Wall -Wextra -Wabi -ggdb")
configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_SOURCE_DIR}/config.hpp" @ONLY)

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
