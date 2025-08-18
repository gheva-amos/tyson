cmake_minimum_required(VERSION 3.14)

add_library(lexer
    string_handler.cpp
    lexer.cpp
    token.cpp)
target_compile_options(lexer PRIVATE -Wall -Wextra -pedantic -Wno-enum-compare -Wno-maybe-uninitialized -Wno-uninitialized -Wno-unused-parameter)
