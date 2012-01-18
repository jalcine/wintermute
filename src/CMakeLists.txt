project(WintermuteCore)
include(WintermuteDefaults)
include(WintermuteMacros)

file(GLOB WINTERMUTE_SOURCES "*.cpp")
file(GLOB WINTERMUTE_HEADERS "*.hpp")
file(GLOB top_HEADERS "*.hpp")
file(GLOB plugins_SOURCES "plugins/*.cpp")
file(GLOB plugins_HEADERS "plugins/*.hpp")
list(APPEND WINTERMUTE_SOURCES ${plugins_SOURCES})
list(APPEND WINTERMUTE_HEADERS ${plugins_HEADERS})
QT4_WRAP_CPP(WINTERMUTE_SOURCES ${WINTERMUTE_HEADERS})

## Targets
add_executable(wintermute ${WINTERMUTE_SOURCES})
target_link_libraries(wintermute ${WINTERMUTE_LIBRARIES})

## Installs
install(TARGETS wintermute
    RUNTIME DESTINATION "${WINTER_BIN_INSTALL_DIR}"
    COMPONENT bin)

install(FILES ${top_HEADERS}
    DESTINATION "${WINTER_INCLUDE_INSTALL_DIR}"
    COMPONENT dev)

install(FILES ${plugins_HEADERS}
    DESTINATION "${WINTER_INCLUDE_INSTALL_DIR}/plugins"
    COMPONENT dev)

