set(app ${PROJECT_NAME})

set(LIBS mylib) 
file(GLOB_RECURSE CODE_FILES *.cpp)

set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost 1.59.0 COMPONENTS system filesystem REQUIRED)

include_directories ("${PROJECT_SOURCE_DIR}/lib" ${Boost_INCLUDE_DIR})
link_directories ("${PROJECT_SOURCE_DIR}/lib" ${Boost_LIBRARY_DIR})

add_executable (${app} ${CODE_FILES})

target_link_libraries (${app} ${LIBS} 
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY} 
)

install (TARGETS ${app}
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR})
