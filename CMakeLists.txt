project(clox)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(clox src/main.c
src/Chunk/chunk.c
src/debug.c
src/memory.c
)
