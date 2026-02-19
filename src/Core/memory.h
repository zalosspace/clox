#pragma once

#include "common.h"

#define ALLOCATE(type, count) \
    (type*)reallocate(NULL, 0, sizeof(type) * (count))

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCapacity, newCapacity) \
(type*)reallocate(pointer, sizeof(type) * (oldCapacity), \
                  sizeof(type) * (newCapacity))

#define FREE_ARRAY(type, pointer, currCapacity) \
(type*)reallocate(pointer, sizeof(type) * (currCapacity), 0)

void* reallocate(void* pointer, size_t oldSize, size_t newSize);
