cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## Installs
install(FILES ${WNTRLING_SYNTAX_HDRS}
    DESTINATION "${WNTRLING_INCLUDE_DIR}/syntax"
    COMPONENT dev)

