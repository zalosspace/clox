#include "common.h"
#include "VM/vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void repl() {
    char line[1024];
    printf("Welcome to Clox: \n");
    while(true) {
        printf(">> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }
        
        interpret(line);
    }
}

static char* readFile(const char *path) {
    // Open in read binary (raw bytes)
    FILE *file = fopen(path, "rb"); 
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74); // I/0 error
    }

    // Get file size
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    // Read file data into buffer
    char *buffer = (char*)malloc(fileSize + 1);
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74); // I/0 error
    }

    buffer[bytesRead] = '\0'; // null byte at the end

    fclose(file);
    return buffer;
}

static void executeFile(const char *path) {
    char *source = readFile(path);

    InterpretResult result = interpret(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR) 
        exit (65); // Data error
    if (result == INTERPRET_RUNTIME_ERROR) 
        exit (70);
}

int main(int argc, const char* argv[]) {
    initVM();

    if (argc == 1) {
        repl();
    }
    else if (argc == 2) {
        executeFile(argv[1]);
    }
    else {
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64); // Command line usage error
    }

    freeVM();

    return 0;
}
