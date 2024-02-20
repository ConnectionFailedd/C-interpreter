execute_process(COMMAND flex --noline -o lexer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/lexer.ll WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/frontend)
execute_process(COMMAND bison --no-lines -o parser.cpp --header=parser.hpp ${CMAKE_CURRENT_SOURCE_DIR}/parser.yy WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/frontend)
# execute_process(COMMAND bison --no-lines -o parser.cpp --header=parser.hpp -Wconflicts-sr -Wcounterexamples ${CMAKE_CURRENT_SOURCE_DIR}/parser.yy WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/frontend)

execute_process(COMMAND clang-format -i parser.hpp WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/frontend)
execute_process(COMMAND clang-format -i lexer.cpp WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/frontend)
execute_process(COMMAND clang-format -i parser.cpp WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/frontend)

add_library(FRONTEND lexer.cpp parser.cpp)