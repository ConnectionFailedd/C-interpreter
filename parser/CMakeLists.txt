execute_process(COMMAND flex --noline -o lexer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/lexer.ll WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/parser_cpp)
execute_process(COMMAND bison --no-lines -o parser.cpp --header=parser.hpp --locations ${CMAKE_CURRENT_SOURCE_DIR}/parser.yy WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/parser_cpp)

execute_process(COMMAND clang-format -i ./parser_cpp/parser.hpp WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
execute_process(COMMAND clang-format -i ./include/location.hpp WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
execute_process(COMMAND clang-format -i ./parser_cpp/lexer.cpp WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
execute_process(COMMAND clang-format -i ./parser_cpp/parser.cpp WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

execute_process(COMMAND mv ./parser_cpp/parser.hpp ./include/parser.hpp WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})