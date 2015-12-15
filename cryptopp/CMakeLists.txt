PROJECT(cryptopp)

cmake_minimum_required(VERSION 2.8)

#enable_language(ASM_MASM)
#set_source_files_properties(foo.s PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")
#SET(ASM_OPTIONS "-x assembler-with-cpp")
#SET(CMAKE_ASM_FLAGS "${CFLAGS} ${ASM_OPTIONS}" )

set(crypt_lib libcryptopp)
set(crypt_app testcrypt)
set(crypt_app_src test.cpp)

file(GLOB lib_src
       ${PROJECT_SOURCE_DIR}/src/*.h
       ${PROJECT_SOURCE_DIR}/src/*.cpp
)

#file(GLOB lib_src
#       ${PROJECT_SOURCE_DIR}/src/*.h
#       ${PROJECT_SOURCE_DIR}/src/*.cpp
#       ${PROJECT_SOURCE_DIR}/src/*.asm
#)

#message("-----lxulxulxu: ${lib_src}")
file(GLOB lib_hdr
       ${PROJECT_SOURCE_DIR}/src/*.h)

list(REMOVE_ITEM lib_src 
	${PROJECT_SOURCE_DIR}/src/${crypt_app_src} 
)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(${crypt_lib} ${lib_src})
#target_compile_definitions(${crypt_lib} PRIVATE "CRYPTOPP_DISABLE_SSSE3" "CRYPTOPP_DISABLE_AESNI" "PREFER_WINDOWS_STYLE_SOCKETS")
#target_compile_definitions(${crypt_lib} PRIVATE "CRYPTOPP_DISABLE_ASM" "CRYPTOPP_DISABLE_SSSE3" "CRYPTOPP_DISABLE_AESNI" "PREFER_WINDOWS_STYLE_SOCKETS" "ALL_RDRAND_INTRIN_AVAILABLE" "ALL_RDSEED_INTRIN_AVAILABLE")
target_compile_definitions(${crypt_lib} PRIVATE "CRYPTOPP_DISABLE_ASM" "CRYPTOPP_DISABLE_SSSE3" "CRYPTOPP_DISABLE_AESNI" "PREFER_WINDOWS_STYLE_SOCKETS" "ALL_RDRAND_INTRIN_AVAILABLE" "ALL_RDSEED_INTRIN_AVAILABLE")

add_executable (${crypt_app} 
		${PROJECT_SOURCE_DIR}/src/${crypt_app_src} 
)
target_compile_definitions(${crypt_app} PRIVATE "CRYPTOPP_DISABLE_ASM" "CRYPTOPP_DISABLE_SSSE3" "CRYPTOPP_DISABLE_AESNI" "PREFER_WINDOWS_STYLE_SOCKETS")
target_link_libraries (${crypt_app} ${crypt_lib})

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include/cryptopp" CACHE PATH "Installation directory for headers")

install (TARGETS ${crypt_lib} ${crypt_app}
         RUNTIME DESTINATION ${INSTALL_BIN_DIR}
         ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
         LIBRARY DESTINATION ${INSTALL_LIB_DIR})

install(FILES ${lib_hdr} 
	DESTINATION "${INSTALL_INC_DIR}")
