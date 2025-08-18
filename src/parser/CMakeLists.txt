cmake_minimum_required(VERSION 3.14)

add_library(parser
    parser.cpp)
target_compile_options(parser PRIVATE -Wall -Wextra -pedantic -Wno-enum-compare -Wno-maybe-uninitialized -Wno-uninitialized -Wno-unused-parameter)
target_link_libraries(parser PRIVATE lexer ast)
