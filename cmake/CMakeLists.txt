cmake_minimum_required(VERSION 2.8)

add_subdirectory(Modules)
add_subdirectory(doc)

file(GLOB TEMPLATE_FILES "${CMAKE_SOURCE_DIR}/cmake/*.in")

install(FILES ${TEMPLATE_FILES}
  DESTINATION "${WINTER_CMAKE_DATA_DIR}")


