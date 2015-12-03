set(test ${CMAKE_PROJECT_NAME}_test)

find_package(GTest REQUIRED)

set(test_src test.cpp)
set(LIBS ${CMAKE_PROJECT_NAME}_lib) 

include_directories ("${PROJECT_SOURCE_DIR}/lib" ${GTEST_INCLUDE_DIRS})
link_directories ("${PROJECT_SOURCE_DIR}/lib")

add_executable (${test} ${test_src})
target_link_libraries (${test} ${LIBS} ${GTEST_BOTH_LIBRARIES})

add_test(Test1 ${test})

install (TARGETS ${test}
         RUNTIME DESTINATION test)
