include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(/opt/homebrew/opt/flex/include)

add_library(PARSER lexer.cpp parser.cpp)