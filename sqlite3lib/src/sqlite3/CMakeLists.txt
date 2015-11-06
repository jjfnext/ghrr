option(WITH_SQLITE_DEBUG    "Build SQLite debug features" OFF)
option(WITH_SQLITE_MEMDEBUG "Build SQLite memory debug features" OFF)
option(WITH_SQLITE_RTREE    "Build R*Tree index extension" OFF)

set (SQLITE_HDR sqlite3.h sqlite3ext.h)
set (SQLITE_SOURCES sqlite3.c ${SQLITE_HDR})
set (SQLITE_BIN_SOURCES shell.c ${SQLITE_SOURCES})
set (SQLITE_LIB sqlite3)
set (SQLITE_DYN_LIB sqlite3dyn)
set (SQLITE_BIN sqlite)

include_directories (.)

add_definitions(-DSQLITE_ENABLE_RTREE)
add_definitions(-DSQLITE_ENABLE_FTS4)
add_definitions(-DSQLITE_ENABLE_FTS5)
add_definitions(-DSQLITE_ENABLE_JSON1)

if(WITH_SQLITE_DEBUG)
    add_definitions(-DSQLITE_DEBUG)
endif()
if(WITH_SQLITE_MEMDEBUG)
    add_definitions(-DSQLITE_MEMDEBUG)
endif()
if(WITH_SQLITE_RTREE)
    add_definitions(-DSQLITE_ENABLE_RTREE)
endif()

add_library(${SQLITE_LIB} ${SQLITE_SOURCES})
add_library(${SQLITE_DYN_LIB} SHARED ${SQLITE_SOURCES})
add_executable(${SQLITE_BIN} ${SQLITE_BIN_SOURCES})

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")

install (TARGETS ${SQLITE_LIB} ${SQLITE_DYN_LIB} ${SQLITE_BIN}
         RUNTIME DESTINATION ${INSTALL_BIN_DIR}
         ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
         LIBRARY DESTINATION ${INSTALL_LIB_DIR})

install(FILES ${SQLITE_HDR} DESTINATION "${INSTALL_INC_DIR}")


