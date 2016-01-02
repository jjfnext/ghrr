set(sqlite3pp_lib ${CMAKE_PROJECT_NAME}_sqlite3pp)

file(GLOB_RECURSE CODE_FILES *.cpp *.h)

set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost 1.59.0 COMPONENTS system filesystem REQUIRED)

message("${SQLITE3_INCLUDE_DIR}----------")

include_directories(. ${SQLITE3_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

add_library(${sqlite3pp_lib} ${CODE_FILES})

