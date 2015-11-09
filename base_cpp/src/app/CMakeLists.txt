set(APP_NAME myrun)

set(APP_SRC main.cpp)

set(LIBS funlib) 

include_directories ("${PROJECT_SOURCE_DIR}/funlib")
link_directories ("${PROJECT_SOURCE_DIR}/funlib")

add_executable (${APP_NAME} ${APP_SRC})
target_link_libraries (${APP_NAME} ${LIBS})

install (TARGETS ${APP_NAME}
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR})
