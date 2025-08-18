cmake_minimum_required(VERSION 3.14)

add_library(ast
    ast.cpp)
target_compile_options(ast PRIVATE -Wall -Wextra -pedantic -Wno-enum-compare -Wno-maybe-uninitialized -Wno-uninitialized -Wno-unused-parameter)
target_link_libraries(ast PRIVATE lexer)
