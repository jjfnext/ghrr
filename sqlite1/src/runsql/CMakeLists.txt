set(Boost_INCLUDE_DIR /Volumes/seagate/bsdprj/install/include)
set(Boost_LIBRARY_DIR /Volumes/seagate/bsdprj/install/lib)
find_package(Boost COMPONENTS REQUIRED)
#include_directories(${Boost_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIR})

#set(Boost_USE_STATIC_LIBS OFF) 
#set(Boost_USE_MULTITHREADED ON)  
#set(Boost_USE_STATIC_RUNTIME OFF) 

#find_package(Boost 1.55.0 COMPONENTS lexical_cast) 

#FIND_PACKAGE(Boost)
#IF (Boost_FOUND)
#    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
#    ADD_DEFINITIONS( "-DHAS_BOOST" )
#ENDIF()

include_directories ("${PROJECT_SOURCE_DIR}/sqlite3" ${Boost_INCLUDE_DIR})

set (APP myrun)
add_executable (${APP} main.cpp)
target_link_libraries (${APP} sqlite3)
install (TARGETS ${APP}
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR})
