#include "common.h"
#include "lexer.h"

#include <ctype.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    const char* start; // Start of new token
    const char* current; // Most recently cosumed lexeme
    int line;
} Lexer;

Lexer lexer;

void initLexer(const char *source) {
    lexer.start = source;
    lexer.current = source;
    lexer.line = 0;
}

static bool isAlpha(char c) {
    return (
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c == '_')
    );
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAtEnd() {
    return *lexer.current == '\0';
}

static char advance() {
    lexer.current++;

    return lexer.current[-1];
}

static char peek() {
    return *lexer.current;
}

static char peekNext() {
    if (isAtEnd()) 
        return '\0';

    return lexer.current[1];
}

static bool match(char expected) {
    if (isAtEnd()) return false;
    if (*lexer.current != expected) return false;
    lexer.current++;

    return true;
}

static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = lexer.start;
    // Length updates as lexer scans it
    token.length = (int)(lexer.current - lexer.start); 
    token.line = lexer.line;

    return token;
}

static Token errorToken(const char *message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message); 
    token.line = lexer.line;

    return token;
}

static void skipWhitespace() {
    while (true) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                lexer.line++;
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    // A
                    while (peek() != '\n' && !isAtEnd()) 
                        advance();
                }
                else {
                    return;
                }
                break;
            default: 
                return;
        }
    }
}

static TokenType checkKeyword(int start, int length,
                              const char *rest, TokenType type) 
{
    if (lexer.current - lexer.start == start + length && 
        memcmp(lexer.start + start, rest, length) == 0) 
    {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

static TokenType identifierType() {
    switch (lexer.start[0]) {
        case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
        case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'f': 
            if (lexer.current - lexer.start > 1) {
                switch (lexer.start[1]) {
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
                }
            }

            break;
        case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
        case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        case 't':
            if (lexer.current - lexer.start > 1) {
                switch (lexer.start[1]) {
                    case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }

    return TOKEN_IDENTIFIER;
}

static Token handleIdentifier() {
    while (isAlpha(peek()) || isDigit(peek())) 
        advance();

    return makeToken(identifierType());
}

static Token handleNumber() {
    while (isDigit(peek())) 
        advance();

    // Look for a fractional part
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume '.'
        advance();

        while (isDigit(peek()))
            advance();
    }

    return makeToken(TOKEN_NUMBER);
}

static Token handleString() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') 
            lexer.line++;
        advance();
    }

    if (isAtEnd()) 
        return errorToken("Unterminated string.");

    // The closing quote.
    advance();
    return makeToken(TOKEN_STRING);
}

Token scanToken() {
    skipWhitespace();
    lexer.start = lexer.current;

    if (isAtEnd()) 
        return makeToken(TOKEN_EOF);

    char c = advance();

    // Allow digits after alpha
    if (isAlpha(c)) return handleIdentifier();

    if (isDigit(c)) return handleNumber();

    switch (c) {
        // Single lexeme token
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);

        // One or two lexeme token
        case '!':
            return makeToken(
                match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return makeToken(
                match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken(
                match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return makeToken(
                match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"': return handleString();
    }

    return errorToken("Unexpected character.");
}
