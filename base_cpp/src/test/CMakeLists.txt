set(TEST_NAME mytest)

set(TEST_SRC test.cpp)

set(LIBS funlib) 

include_directories ("${PROJECT_SOURCE_DIR}/funlib")
link_directories ("${PROJECT_SOURCE_DIR}/funlib")

add_executable (${TEST_NAME} ${TEST_SRC})
target_link_libraries (${TEST_NAME} ${LIBS})

install (TARGETS ${TEST_NAME}
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR})
