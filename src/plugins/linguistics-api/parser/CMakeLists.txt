cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## Installs
install(FILES ${WNTRLING_PARSER_HDRS}
    DESTINATION "${WNTRLING_INCLUDE_DIR}/parser"
    COMPONENT dev)

