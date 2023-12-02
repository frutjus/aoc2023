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

bool peek(const char **str, bool *f(const char **str)) {
    const char *curstr = *str;
    return f(&curstr);
}