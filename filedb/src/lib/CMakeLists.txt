set(lib ${CMAKE_PROJECT_NAME}_lib)
set(sqlite3pp_lib ${CMAKE_PROJECT_NAME}_sqlite3pp)

message("${sqlite3pp_lib}-----------")

file(GLOB_RECURSE CODE_FILES *.cpp *.hpp)

set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost 1.59.0 COMPONENTS system filesystem REQUIRED)

FIND_LIBRARY(sqlite3_lib NAMES sqlite3 ${SQLITE3_LIBRARY_DIR})

#FIND_LIBRARY(sqlite3_lib NAMES sqlite3 ${SQLITE3_LIBRARY_DIR} PATH_SUFFIXES lib)
message("${sqlite3_lib}-----------")

FIND_LIBRARY(cryptopp_lib NAMES libcryptopp ${DCRYPTOPP_LIBRARY_DIR})

include_directories(. ${PROJECT_SOURCE_DIR}/sqlite3pp ${SQLITE3_INCLUDE_DIR} ${Boost_INCLUDE_DIR}
${CRYPTOPP_INCLUDE_DIR} ${Qt5Core_INCLUDE_DIRS}
)

link_directories ("${PROJECT_SOURCE_DIR}/sqlite3pp" ${SQLITE3_LIBRARY_DIR}
${CRYPTOPP_LIBRARY_DIR} 
)

#add_library(${lib} ${sqlite3pp_lib} ${sqlite3_lib} ${CODE_FILES})
add_library(${lib} ${CODE_FILES})

target_link_libraries (${lib} ${sqlite3pp_lib} ${sqlite3_lib}
${cryptopp_lib}
)

#link_directories ("${PROJECT_SOURCE_DIR}/sqlite3pp")

