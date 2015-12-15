set(CMAKE_INSTALL_PREFIX dist CACHE STRING "Install path prefix.")

PROJECT(simple_crypto)

cmake_minimum_required(VERSION 3.3)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(app)
