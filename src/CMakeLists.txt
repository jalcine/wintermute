project(WintermuteCore)

file(GLOB WINTERMUTE_SOURCES "*.cpp")
file(GLOB WINTERMUTE_MOC_FILES "*.hpp")
QT_WRAP_CPP(WINTERMUTE_SOURCES ${WINTERMUTE_MOC_FILES})

#
# Targets
#
add_executable(wintermute ${WINTERMUTE_SOURCES})
target_link_libraries(wintermtue ${WINTERMUTE_LIBRARIES})
