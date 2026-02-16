#include "chunk.h"
#include "../memory.h"

#include <stdlib.h>

void initChunk(Chunk *chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
}

void writeChunk(Chunk *chunk, uint8_t byte) {
    // Check if current array has 
    // capacity for new byte
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY( 
            uint8_t,
            chunk->code,
            oldCapacity, 
            chunk->capacity
        );
    }
    
    // Append new byte
    chunk->code[chunk->count] = byte;
    chunk->count++;
}

void freeChunk(Chunk *chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    initChunk(chunk);
}
