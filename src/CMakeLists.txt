cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
  replxx
  GIT_REPOSITORY https://github.com/AmokHuginnsson/replxx.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(replxx)

add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(ast)
add_subdirectory(lisp)

add_executable(tyson repl.cpp)
target_compile_options(tyson PRIVATE -Wall -Wextra -pedantic -Wno-enum-compare -Wno-maybe-uninitialized -Wno-uninitialized -Wno-unused-parameter)
set_target_properties(tyson PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin")
target_link_libraries(tyson PRIVATE replxx::replxx)

add_executable(experiments experiments.cpp)
target_compile_options(experiments PRIVATE -Wall -Wextra -pedantic -Wno-enum-compare -Wno-maybe-uninitialized -Wno-uninitialized -Wno-unused-parameter)
set_target_properties(experiments PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin")
target_link_libraries(experiments PRIVATE )
target_link_libraries(experiments lexer parser ast lisp)
