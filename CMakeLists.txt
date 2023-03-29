cmake_minimum_required(VERSION 3.0.0)
project(CINT VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(CINT main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++17")

execute_process(COMMAND opt/homebrew/opt/flex/bin/flex -o ${CMAKE_CURRENT_SOURCE_DIR}/parser_cpp/lexer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/parser/lexer.ll)
execute_process(COMMAND opt/homebrew/opt/bison/bin/bison --header=${CMAKE_CURRENT_SOURCE_DIR}/include/parser.hpp -o ${CMAKE_CURRENT_SOURCE_DIR}/parser_cpp/parser.cpp ${CMAKE_CURRENT_SOURCE_DIR}/parser/parser.yy)

add_subdirectory(parser_cpp)