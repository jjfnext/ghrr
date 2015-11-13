PROJECT(lua)

cmake_minimum_required(VERSION 2.8)

set(lua_dir lua-5.3.1)
set(lua_lib liblua)
set(lua_app lua)
set(luac_app luac)
set(lua_app_src lua.c)
set(luac_app_src luac.c)
set(lua_hdr lua.h)

file(GLOB lib_src
       ${PROJECT_SOURCE_DIR}/${lua_dir}/src/*.h
       ${PROJECT_SOURCE_DIR}/${lua_dir}/src/*.c)

list(REMOVE_ITEM lib_src 
	${PROJECT_SOURCE_DIR}/${lua_dir}/src/{$lua_app_src}
	${PROJECT_SOURCE_DIR}/${lua_dir}/src/{$luac_app_src}
)

include_directories(${PROJECT_SOURCE_DIR}/${lua_dir}/src)

add_library(${lua_lib} ${lib_src})
add_executable (${lua_app} 
		${PROJECT_SOURCE_DIR}/${lua_dir}/src/${lua_app_src} 
)
target_link_libraries (${lua_app} ${lua_lib})

add_executable (${luac_app} 	
		${PROJECT_SOURCE_DIR}/${lua_dir}/src/${luac_app_src} 
)
target_link_libraries (${luac_app} ${lua_lib})

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")

install (TARGETS ${lua_lib} ${lua_app} ${luac_app}
         RUNTIME DESTINATION ${INSTALL_BIN_DIR}
         ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
         LIBRARY DESTINATION ${INSTALL_LIB_DIR})

install(FILES ${PROJECT_SOURCE_DIR}/${lua_dir}/src/${lua_hdr} 
	DESTINATION "${INSTALL_INC_DIR}")



