execute_process(COMMAND flex -o lexer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/lexer.ll WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/parser_cpp)
execute_process(COMMAND bison -o parser.cpp --header=parser.hpp --locations ${CMAKE_CURRENT_SOURCE_DIR}/parser.yy WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/parser_cpp)

execute_process(COMMAND clang-format -i ${CMAKE_SOURCE_DIR}/parser_cpp/parser.hpp)
execute_process(COMMAND clang-format -i ${CMAKE_SOURCE_DIR}/parser_cpp/location.hh)
execute_process(COMMAND clang-format -i ${CMAKE_SOURCE_DIR}/parser_cpp/lexer.cpp)
execute_process(COMMAND clang-format -i ${CMAKE_SOURCE_DIR}/parser_cpp/parser.cpp)

execute_process(COMMAND mv ./parser_cpp/parser.hpp ./include/parser.hpp WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
execute_process(COMMAND mv ./parser_cpp/location.hh ./include/location.hh WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})