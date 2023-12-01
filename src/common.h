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

