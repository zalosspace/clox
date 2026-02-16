#pragma once

#include "../common.h"

typedef enum {
    OP_RETURN,
} OpCode;

typedef struct {
    // number of element in use in array we have allocated
    int count;
    // number of element in array we have allocated
    int capacity; 
    uint8_t* code; // hence, a bytecode
} Chunk;

void initChunk(Chunk *chunk);
void freeChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte);
