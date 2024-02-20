cmake_minimum_required(VERSION 3.6.0)
project(CINT VERSION 0.1.0)

add_executable(CINT main.cpp)

include_directories(frontend)
include_directories(ir)
include_directories(backend)
include_directories(symbol_table)
include_directories(any)

add_subdirectory(frontend)
add_subdirectory(ir)
add_subdirectory(backend)
add_subdirectory(symbol_table)

target_link_libraries(CINT FRONTEND IR BACKEND SYMBOL_TABLE)
