#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char *contents;
    size_t size;
} String;

String read_file(char *path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        printf("Error; could not open file: %s\n", f);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f) + 1;
    char *contents = (char*)malloc(size);
    if (!contents) {
        printf("Error; could not allocate %d bytes of memory\n", size);
        exit(1);
    }

    fseek(f, 0, SEEK_SET);
    fread(contents, 1, size - 1, f);
    contents[size] = 0;

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

/* --- Parsing --- */

int parse_num(const char **str) {
    int num = 0;

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

bool parse_newline(const char **str) {
    return parse_string(str, "\r\n") || parse_string(str, "\r") || parse_string(str, "\n");
}

bool peek(const char *str, bool f(const char **str)) {
    const char *curstr = str;
    return f(&curstr);
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

    char **contents = (char**)malloc(height * sizeof(char*));
    
    if (!contents) {
        printf("Error; could not allocate %d bytes of memory\n", height * sizeof(char*));
        exit(1);
    }

    for (int i = 0; i < height; i++) {
        contents[i] = (char*)malloc(width);
        if (!contents[i]) {
            printf("Error; could not allocate %d bytes of memory\n", width);
            exit(1);
        }
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


