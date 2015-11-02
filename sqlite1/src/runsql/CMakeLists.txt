include_directories ("${PROJECT_SOURCE_DIR}/sqlite3")

set (APP myrun)
add_executable (${APP} main.cpp)
target_link_libraries (${APP} sqlite3)
install (TARGETS ${APP}
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR})
