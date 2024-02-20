info flex   # usage
man flex    # manual
info bison  # usage
man bison   # manual
bison -o parser.cpp --header=parser.hpp  parser.yy
flex -o lexer.cpp lexer.ll