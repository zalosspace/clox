#pragma once

#include "Chunk/chunk.h"
#include "Core/value.h"

#define STACK_MAX 256

typedef struct {
    Chunk *chunk;
    uint8_t* ip; // Instruction Pointer
    Value stack[STACK_MAX];
    Value* stackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(Chunk *chunk);
void push(Value value);
Value pop();
