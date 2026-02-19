cmake_minimum_required(VERSION 3.16)

project(clox C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(clox
    src/main.c
    src/Frontend/compiler.c
    src/Frontend/lexer.c
    src/Chunk/chunk.c
    src/Debug/debug.c
    src/Core/memory.c
    src/Core/object.c
    src/Core/value.c
    src/VM/vm.c
)

target_include_directories(clox PRIVATE src)
