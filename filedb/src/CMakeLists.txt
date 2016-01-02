set(CMAKE_INSTALL_PREFIX dist CACHE STRING "Install path prefix.")

PROJECT(filedb)

cmake_minimum_required(VERSION 3.3)

enable_testing()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Test REQUIRED)

set(TEST_LINK_LIBRARIES Qt5::Test)

add_subdirectory(sqlite3pp)
add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(test)

set(CPACK_GENERATOR NSIS)
set(CPACK_PACKAGE_NAME "filedb")
set(CPACK_PACKAGE_VENDOR "coolnu.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "FileLst BY Coolnu")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "FileDB")
SET(CPACK_NSIS_MODIFY_PATH ON)

INCLUDE(CPack)
