#pragma once

#include "Core/value.h"
#include "common.h"

typedef enum {
    OP_CONSTANT,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NOT,
    OP_NEGATE,
    OP_RETURN,
} OpCode;

typedef struct {
    // Number of element in use in array we have allocated
    int count;
    // Number of element in array we have allocated
    int capacity; 
    uint8_t* code; // Hence, a bytecode
    int* lines;
    ValueArray constants; // Constant pool
} Chunk;

void initChunk(Chunk *chunk);
void freeChunk(Chunk *chunk);
// Writes opcodes or operands
void writeChunk(Chunk *chunk, uint8_t byte, int line);
int addConstant(Chunk *chunk, Value value);
