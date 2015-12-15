set(app ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE CODE_FILES *.cpp *.hpp)

FIND_LIBRARY(cryptopp_lib NAMES libcryptopp ${CRYPTOPP_LIBRARY_DIR})

include_directories(${CRYPTOPP_INCLUDE_DIR})
link_directories(${CRYPTOPP_LIBRARY_DIR})

add_executable (${app} 
  ${CODE_FILES}
)

target_link_libraries (${app} ${cryptopp_lib})

install (TARGETS ${app}
         RUNTIME DESTINATION .)
