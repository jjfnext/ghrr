set(lib mylib)
set(lib_src fun.hpp fun.cpp)

include_directories(.)

add_library(${lib} ${lib_src})

