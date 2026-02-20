#include "memory.h"
#include "VM/vm.h"

#include <stdlib.h>

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }

    void *result = realloc(pointer, newSize);

    if (result == NULL)
        exit(1); // return due to memory allocation error

    return result;
}

static void freeObject(Obj *object) {
    switch (object->type) {
        case OBJ_STRING: {
            // ObjString owns heap char buffer, so we free it first
            ObjString *string = (ObjString*)object;
            FREE_ARRAY(char, string->chars, string->length + 1);

            FREE(ObjString, object);
            break;
        }
    }
}

void freeObjects() {
    Obj *object = vm.objects;

    // Iterate through Obj linked list
    while (object != NULL) {
        Obj *next = object->next;
        freeObject(object);
        object = next;
    }
}
