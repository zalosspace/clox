#include "debug.h"
#include "Chunk/chunk.h"

#include <stdio.h>

void disassembleChunk(Chunk *chunk, const char* name) {
    printf("== %s ==\n", name);

    for (int offset=0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstrution(const char* name, int offset) {
    printf("%s\n", name);

    return offset + 1;
}

int disassembleInstruction(Chunk *chunk, int offset) {
    printf("%04d ", offset);

    uint8_t instruction = chunk->code[offset];

    switch (instruction) {
        case OP_RETURN:
            return simpleInstrution("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}

