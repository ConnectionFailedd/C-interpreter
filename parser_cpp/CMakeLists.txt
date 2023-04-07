include_directories(${CMAKE_SOURCE_DIR}/include/)

add_library(PARSER lexer.cpp parser.cpp driver.cpp)