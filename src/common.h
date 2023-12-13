#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

typedef unsigned long long int ull;

#define iter(i, n) for (int i = 0; i < (n); i++)

void *myalloc(size_t bytes) {
    void *p = malloc(bytes);
    if (!p) {
        fprintf(stderr, "Error; could not allocate %llu bytes of memory\n", bytes);
        exit(1);
    }
    return p;
}

typedef struct {
    ull *at;
    size_t size;
} ArrayInt;

char *read_file(char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "Error; could not open file: %s\n", path);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    char *contents = (char*)myalloc(size+1);

    //printf("file size = %lld\n", size);

    fseek(f, 0, SEEK_SET);
    fread(contents, 1, size, f);
    contents[size] = '\0';

    fclose(f);

    return contents;
}

bool isPrefixOf(const char* str1, const char* str2) {
    for (int i = 0; str1[i] != '\0'; i++) {
        if (str1[i] != str2[i]) return false;
    }
    return true;
}

int charToDigit(char c) {
    return (int)c - 48;
}

bool ArrayInt_contains(ArrayInt arr, int val) {
    for (int i = 0; i < arr.size; i++) {
        if (arr.at[i] == val)
            return true;
    }
    return false;
}

void ArrayInt_push(ArrayInt *arr, ull elem) {
    arr->size++;
    void *_old = arr->at;
    arr->at = myalloc(sizeof(ull) * arr->size);
    memcpy(arr->at, _old, sizeof(ull) * arr->size);
    free(_old);
    arr->at[arr->size] = elem;
}

/* --- Parsing --- */

long long int parse_num(const char **str) {
    long long int num = 0;
    bool negate = false;

    if (**str == '-') {
        (*str)++;
        negate = true;
    }

    while (isdigit(**str)) {
        num = num * 10 + charToDigit(**str);
        (*str)++;
    }

    return num * (negate ? -1 : 1);
}

ull parse_unum(const char **str) {
    ull num = 0;

    while (isdigit(**str)) {
        num = num * 10 + charToDigit(**str);
        (*str)++;
    }

    return num;
}

bool parse_string(const char **str, const char *substr) {
    int i;
    for (i = 0; substr[i] != '\0'; i++) {
        if (substr[i] != (*str)[i]) {
            return false;
        }
    }
    *str += i;

    return true;
}

bool parse_space(const char **str) {
    return parse_string(str, " ");
}

bool parse_spaces(const char **str) {
    bool found = false;
    while (parse_space(str)) found = true;
    return found;
}

bool parse_newline(const char **str) {
    return parse_string(str, "\r\n") || parse_string(str, "\r") || parse_string(str, "\n");
}

bool peek(const char *str, bool f(const char **str)) {
    const char *curstr = str;
    return f(&curstr);
}

ArrayInt parse_nums(const char **str, bool f(const char **str)) {
    ull size = 0;
    const char *curstr = *str;

    // first figure out how large the array should be
    while (isdigit(*curstr)) {
        parse_num(&curstr);
        f(&curstr);
        size++;
    }

    ull *contents = (ull*)malloc(size * sizeof(ull));

    // then actually read the numbers in
    for (int i = 0; i < size; i++) {
        contents[i] = parse_num(str);
        f(str);
    }

    return (ArrayInt) { .at = contents, .size = size };
}

int countLines(const char *str) {
    const char *curstr = str;
    int lines = 0;
    while (*curstr != '\0') {
        if (parse_newline(&curstr)) {
            lines++;
        } else {
            curstr++;
        }
    }
    return lines;
}

bool skip_until(const char **str, bool f(const char **str)) {
    while (**str != '\0') {
        if (f(str)) {
            return true;
        }
        (*str)++;
    }
    return false;
}

bool parse_comma(const char **str) {
    if (**str == ',') {
        (*str)++;
        return true;
    }
    return false;
}

/* --- Grid --- */

typedef struct {
    char **at;
    int width, height;
} Grid;

void measureDimensions(const char *grid, int *width, int *height) {
    *width = 0;
    *height = 0;
    for (int i = 0; !peek(grid + i, parse_newline); i++) {
        (*width)++;
    }
    *height = countLines(grid);
}

Grid gridFromString(char *str) {
    int width = 0, height = 0;
    measureDimensions(str, &width, &height);

    char **contents = (char**)myalloc(height * sizeof(char*));
    

    for (int i = 0; i < height; i++) {
        contents[i] = (char*)myalloc(width);
    }

    int r = 0, c = 0;
    for (const char *curstr = str; *curstr != '\0'; curstr++) {
        if (parse_newline(&curstr)) {
            r++;
            c = 0;
        }
        if (r < height && c < width) {
            contents[r][c] = *curstr;
            c++;
        }
    }

    //free(str.contents);

    return (Grid) { contents, width, height };
}

void printGrid(Grid grid) {
    for (int r = 0; r < grid.height; r++) {
        for (int c = 0; c < grid.width; c++) {
            printf("%c", grid.at[r][c]);
        }
        printf("\n");
    }
}

Grid copyGrid(Grid grid) {
    char** new_grid;

    new_grid = (char**)myalloc(grid.height * sizeof(char*));

    iter(i, grid.height) {
        new_grid[i] = (char*)myalloc(grid.width);
        memcpy(new_grid[i], grid.at[i], grid.width);
    }

    return (Grid) { new_grid, grid.width, grid.height };
}

Grid transposeGrid(Grid grid) {
    char **new_grid = (char**)myalloc(grid.width * sizeof(char*));

    iter(i, grid.width) {
        new_grid[i] = (char*)myalloc(grid.height);
        
        iter(j, grid.height) {
            new_grid[i][j] = grid.at[j][i];
        }
    }

    return (Grid) { new_grid, grid.height, grid.width };
}

void deleteGrid(Grid grid) {
    iter(i, grid.height) {
        free(grid.at[i]);
    }
    free(grid.at);
}

#endif
