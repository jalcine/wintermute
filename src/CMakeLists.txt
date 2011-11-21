project(WintermuteCore)

include(WintermuteDefaults)
include(WintermuteMacros)

file(GLOB WINTERMUTE_SOURCES "*.cpp")
file(GLOB WINTERMUTE_HEADERS "*.hpp")
QT4_WRAP_CPP(WINTERMUTE_SOURCES ${WINTERMUTE_HEADERS})

## Targets
add_executable(wintermute ${WINTERMUTE_SOURCES})
target_link_libraries(wintermute ${WINTERMUTE_LIBRARIES})

## Installs
install(TARGETS wintermute
    RUNTIME DESTINATION "${WINTER_BIN_INSTALL_DIR}"
    COMPONENT bin)

install(FILES ${WINTERMUTE_HEADERS}
    DESTINATION "${WINTER_INCLUDE_INSTALL_DIR}"
    COMPONENT dev)

