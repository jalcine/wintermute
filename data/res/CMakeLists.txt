cmake_minimum_required(VERSION 2.8)

install(FILES "doc.css" "head.htm" "foot.htm" "layout.xml"
        DESTINATION "${WINTER_CMAKE_DIR}/doc")

install(FILES "wintermute.png"
        DESTINATION "${WINTER_CMAKE_DIR}/res")