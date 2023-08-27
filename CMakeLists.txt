cmake_minimum_required(VERSION 3.0.0)
project(CINT VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(CINT main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++17 -g")

include_directories(include)
add_subdirectory(parser)
add_subdirectory(parser_cpp)
add_subdirectory(utils)
add_subdirectory(value)
add_subdirectory(syntax_tree)
add_subdirectory(function)
add_subdirectory(init)

target_link_libraries(CINT UTILS VALUE INIT FUNCTION)
target_link_libraries(CINT SYNTAX_TREE)