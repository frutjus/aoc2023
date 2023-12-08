#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

typedef unsigned long long int ull;

#define iter(i, n) for (int i = 0; i < n; i++)

void *myalloc(size_t bytes) {
    void *p = malloc(bytes);
    if (!p) {
        fprintf(stderr, "Error; could not allocate %zu bytes of memory\n", bytes);
        exit(1);
    }
    return p;
}

typedef struct {
    char *contents;
    size_t size;
} String;

typedef struct {
    ull *contents;
    size_t size;
} ArrayInt;

String read_file(char *path) {
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

    return (String) { .contents = contents, .size = size };
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
        if (arr.contents[i] == val)
            return true;
    }
    return false;
}

/* --- Parsing --- */

ull parse_num(const char **str) {
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

    return (ArrayInt) { .contents = contents, .size = size };
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

/* --- Grid --- */

typedef struct {
    char **contents;
    int width, height;
} Grid;

void measureDimensions(const String grid, int *width, int *height) {
    *width = 0;
    for (int i = 0; !peek(grid.contents + i, parse_newline); i++) {
        (*width)++;
    }
    for (int i = 0; i < grid.size; i++) {
        if (peek(grid.contents + i, parse_newline)) {
            (*height)++;
        }
    }
}

Grid gridFromString(String str) {
    int width = 0, height = 0;
    measureDimensions(str, &width, &height);

    char **contents = (char**)myalloc(height * sizeof(char*));
    

    for (int i = 0; i < height; i++) {
        contents[i] = (char*)myalloc(width);
    }

    int r = 0, c = 0;
    for (const char *curstr = str.contents; *curstr != '\0'; curstr++) {
        if (parse_newline(&curstr)) {
            r++;
            c = 0;
        }
        if (r < height && c < width) {
            contents[r][c] = *curstr;
            c++;
        }
    }

    free(str.contents);

    return (Grid) { contents, width, height };
}

void printGrid(Grid grid) {
    for (int r = 0; r < grid.height; r++) {
        for (int c = 0; c < grid.width; c++) {
            printf("%c", grid.contents[r][c]);
        }
        printf("\n");
    }
}

#endif
