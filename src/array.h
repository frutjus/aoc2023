#ifndef ARRAY_H
#define ARRAY_H

#include <string.h>

#include "common.h"

#define ARRAY_MIN_SIZE 8
#define ARRAY_SIZE_INCREASE 1.5

#define Array(type) struct { type *at; size_t size; size_t capacity; }

#define Array_new(type) { .at = (type*)myalloc(sizeof(type) * ARRAY_MIN_SIZE), .size = 0, .capacity = ARRAY_MIN_SIZE }

#define Array_push(array, elem) { \
    if ((array).size == (array).capacity) { \
        (array).capacity = (array).size * ARRAY_SIZE_INCREASE; \
        void *_old = (array).at; \
        (array).at = myalloc(sizeof((array).at[0]) * (array).capacity); \
        memcpy((array).at, _old, sizeof((array).at[0]) * (array).size); \
        free(_old); \
    } \
    (array).at[array.size++] = elem; \
}



#endif