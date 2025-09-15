cmake_minimum_required(VERSION 3.10)
project(Bytecode)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(Bytecode
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/compiler.cpp
    src/bytecode.cpp
    src/vm.cpp
)
