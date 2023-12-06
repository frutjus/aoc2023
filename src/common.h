#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned long long int ull;

void *myalloc(size_t bytes) {
    void *p = malloc(bytes);
    if (!p) {
        printf("Error; could not allocate %lld bytes of memory\n", bytes);
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
        printf("Error; could not open file: %s\n", path);
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

/* --- Linked List --- */

typedef struct _List {
    void *contents;
    struct _List *next;
} List;

List List_new() {
    return (List) { .contents = 0, .next = 0 };
}

List *List_newm() {
    List *list = (List*)myalloc(sizeof(List));
    *list = List_new();
    return list;
}

bool List_isempty(List list) {
    return list.contents == 0;
}

void List_append(List *list, void *item) {
    
    if (List_isempty(*list)) {
        List *newnode = List_newm();

        list->contents = item;
        list->next = newnode;
    } else {
        List_append(list->next, item);
    }
}

size_t List_size(List list) {
    if (List_isempty(list)) {
        return 0;
    } else {
        return 1 + List_size(*list.next);
    }
}

void List_appendList(List *list1, List *list2) {
    if (List_isempty(*list1)) {
        list1->contents = list2->contents;
        list1->next = list2->next;
    } else {
        List_appendList(list1->next, list2);
    }
}

void List_insert(List *list, void *item) {
    List *newnode = List_newm();
    
    if (List_isempty(*list)) {
        list->contents = item;
        list->next = newnode;
    } else {
        newnode->contents = item;
        newnode->next = list->next;
        list->next = newnode;
    }
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
    int size = 0;
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


