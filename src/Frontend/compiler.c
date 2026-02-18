#include "compiler.h"
#include "Chunk/chunk.h"
#include "VM/vm.h"
#include "lexer.h"
#include "Frontend/lexer.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;

Parser parser;
Chunk *compilingChunk;

static Chunk *currentChunk() {
    return compilingChunk;
}

static void errorAt(Token *token, const char *message) {
    if (parser.panicMode) return;
    parser.panicMode = true; // But Don't Panic

    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    }
    else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);

    parser.hadError = true;
}

static void error(const char *message) {
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const char *message) {
    errorAt(&parser.current, message);
}

static void advance() {
    parser.previous = parser.current;

    while (true) {
        parser.current = scanToken(); 
        // skip if no error
        if (parser.current.type != TOKEN_ERROR) break;

        errorAtCurrent(parser.current.start);
    }
}

static void consume(TokenType type, const char *message) {
    if (parser.current.type == type) {
        advance();

        return;
    }

    errorAtCurrent(message);
}

static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}


static void emitReturn() {
    emitByte(OP_RETURN);
}

static void endCompiler() {
    emitReturn();
}

bool compile(const char *source, Chunk *chunk) {
    initLexer(source);
    compilingChunk = chunk;

    parser.hadError = false;
    parser.panicMode = false;

    advance();
    // expression();
    consume(TOKEN_EOF, "Expect end of expression");

    int line = -1;
    while (true) {
        Token token = scanToken();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        }
        else {
            printf("   | ");
        }

        printf("%2d '%.*s'\n", token.type, token.length, token.start);

        if (token.type == TOKEN_EOF) break;
    }

    endCompiler();
    return !parser.hadError; 
}
