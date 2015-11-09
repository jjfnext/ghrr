set(FUN_LIB_NAME funlib)
set(FUN_LIB_SRC fun.hpp fun.cpp)

include_directories(.)

add_library(${FUN_LIB_NAME} ${FUN_LIB_SRC})

