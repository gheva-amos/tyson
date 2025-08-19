cmake_minimum_required(VERSION 3.14)

add_library(lisp
    atom_table.cpp
    frame.cpp
    runtime_types.cpp
    env.cpp
    primitives.cpp)
target_compile_options(lisp PRIVATE -Wall -Wextra -pedantic -Wno-enum-compare -Wno-maybe-uninitialized -Wno-uninitialized -Wno-unused-parameter)
