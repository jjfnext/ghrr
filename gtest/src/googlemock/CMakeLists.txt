set(gmock_lib gmock)
set(gmock_main_lib gmock_main)

file(GLOB_RECURSE all_hdr ${PROJECT_SOURCE_DIR}/googlemock/include/*.h)

set(lib_src ${all_hdr} ${PROJECT_SOURCE_DIR}/googlemock/src/gmock-all.cc)
set(main_src ${all_hdr} ${PROJECT_SOURCE_DIR}/googlemock/src/gmock_main.cc)

include_directories(${PROJECT_SOURCE_DIR}/googletest/include
					${PROJECT_SOURCE_DIR}/googlemock/include
					${PROJECT_SOURCE_DIR}/googlemock)

add_library(${gmock_lib} ${lib_src})
add_library(${gmock_main_lib} ${main_src})

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")

install(TARGETS ${gmock_lib} ${gmock_main_lib} 
         RUNTIME DESTINATION ${INSTALL_BIN_DIR}
         ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
         LIBRARY DESTINATION ${INSTALL_LIB_DIR})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/googlemock/include/gmock DESTINATION "${INSTALL_INC_DIR}")


