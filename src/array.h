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
    (array).at[(array).size++] = elem; \
}

#define Array_last(array) ((array).at[(array).size-1])

#define Array_sort(type, array, f) { \
    bool sorted; \
    do { \
        sorted = true; \
        for(int i = 0; i < (array).size - 1; i++) { \
            if (f((array).at[i], (array).at[i+1]) > 0) { \
                sorted = false; \
                type e = (array).at[i]; \
                (array).at[i] = (array).at[i+1]; \
                (array).at[i+1] = e; \
            } \
        } \
    } while (!sorted); \
}

#define Array_reverse(type, array) { \
    type *_old = (array).at; \
    (array).at = myalloc(sizeof((array).at[0]) * (array).capacity); \
    for (int i = 0; i < (array).size; i++) { \
        (array).at[i] = _old[(array).size - i - 1]; \
    } \
    free(_old); \
}

#endif