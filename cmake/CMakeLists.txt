cmake_minimum_required(VERSION 2.8)

include(WintermuteDefaults)

file(GLOB TEMPLATE_FILES "${CMAKE_SOURCE_DIR}/cmake/*.in")

## Installs
install(FILES ${TEMPLATE_FILES}
        DESTINATION "${WINTER_CMAKE_DIR}")

add_subdirectory(Modules)
add_subdirectory(doc)