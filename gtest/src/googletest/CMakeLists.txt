set(gtest_lib gtest)
set(gtest_main_lib gtest_main)

file(GLOB_RECURSE all_hdr ${PROJECT_SOURCE_DIR}/googletest/include/*.h)

set(lib_src ${all_hdr} ${PROJECT_SOURCE_DIR}/googletest/src/gtest-all.cc)
set(main_src ${all_hdr} ${PROJECT_SOURCE_DIR}/googletest/src/gtest_main.cc)

include_directories(${PROJECT_SOURCE_DIR}/googletest/include
					${PROJECT_SOURCE_DIR}/googletest)

add_library(${gtest_lib} ${lib_src})
add_library(${gtest_main_lib} ${main_src})

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")

install(TARGETS ${gtest_lib} ${gtest_main_lib} 
         RUNTIME DESTINATION ${INSTALL_BIN_DIR}
         ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
         LIBRARY DESTINATION ${INSTALL_LIB_DIR})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/googletest/include/gtest DESTINATION "${INSTALL_INC_DIR}")


