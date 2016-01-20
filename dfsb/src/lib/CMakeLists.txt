set(lib ${CMAKE_PROJECT_NAME}_lib)

file(GLOB_RECURSE CODE_FILES *.cpp *.hpp)

set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost 1.59.0 COMPONENTS system filesystem REQUIRED)

include_directories(. ${Boost_INCLUDE_DIR} ${Qt5Core_INCLUDE_DIRS})

add_library(${lib} ${CODE_FILES})

