#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"

typedef struct _List {
    char *key;
    int value;
    struct _List *tail;
} List;

List List_new() {
    return (List) { .key = 0, .value = -1, .tail = 0 };
}

List *List_newm() {
    List *list = (List*)myalloc(sizeof(List));
    *list = List_new();
    return list;
}

bool List_isempty(List list) {
    return list.tail == 0;
}

void List_insert(List *list, char *key, int value) {
    if (List_isempty(*list)) {
        List *tail = List_newm();
        list->key = key;
        list->value = value;
        list->tail = tail;
    } else if (strcmp(list->key, key) == 0) {
        list->value = value;
    } else {
        List_insert(list->tail, key, value);
    }
}

void List_delete(List *list, char *key) {
    if (List_isempty(*list)) {
        return;
    } else if (strcmp(list->key, key) == 0) {
        List *tail = list->tail;
        free(list->key);
        list->key = tail->key;
        list->value = tail->value;
        list->tail = tail->tail;
        free(tail);
    } else {
        List_delete(list->tail, key);
    }
}

#define MAP_BUCKETS 256

typedef struct {
    List buckets[MAP_BUCKETS];
} Hashmap;

Hashmap Map_new() {
    Hashmap map;

    iter(i, MAP_BUCKETS) {
        map.buckets[i] = List_new();
    }

    return map;
}

int hash(char* str) {
    int current_value = 0;

    while (*str != '\0') {
        current_value = ((current_value + *str) * 17) % MAP_BUCKETS;
        str++;
    }

    return current_value;
}

void Map_insert(Hashmap *map, char *key, int value) {
    int keyhash = hash(key);

    List_insert(&map->buckets[keyhash], key, value);
}

void Map_delete(Hashmap *map, char *key) {
    int keyhash = hash(key);

    List_delete(&map->buckets[keyhash], key);
}

char *parse_key(const char **str) {
    char *key;
    int len = 0;

    while (isalpha(*(*str + len))) {
        len++;
    }

    key = (char*)myalloc(len + 1);

    iter(i, len) {
        key[i] = **str;
        (*str)++;
    }
    key[len] = '\0';

    return key;
}

typedef enum {
    invalid,
    insert,
    delete,
} Instruction;

Instruction parse_instruction(const char **str) {
    switch (**str) {
        case '-':
            (*str)++;
            return delete;
        case '=':
            (*str)++;
            return insert;
        default:
            return invalid;
    }
}

ull focusing_power(Hashmap *map) {
    ull power = 0;
    
    for (ull box_num = 0; box_num < MAP_BUCKETS; box_num++) {
        ull slot_num = 1;
        List slot = map->buckets[box_num];

        while (!List_isempty(slot)) {
            power += (box_num + 1) * slot_num * (ull)slot.value;
            slot = *slot.tail;
            slot_num++;
        }
    }

    return power;
}

int main() {
    const char *str = read_file("inp/day15.txt");

    Hashmap map = Map_new();

    while (*str != '\0' && !parse_newline((const char**)&str)) {

        char *key = parse_key(&str);
        Instruction instr = parse_instruction(&str);

        if (instr == insert) {
            int value = parse_num(&str);

            //printf("Inserting %s=%d\n", key, value);

            Map_insert(&map, key, value);

        } else if (instr == delete) {
            //printf("Deleting %s\n", key);

            Map_delete(&map, key);

        } else {
            printf("Invalid instruction: [%c/%d]", instr, instr);
        }

        parse_comma(&str);
    }

    ull ans = focusing_power(&map);

    printf("Answer = [32m%llu[0m\n", ans);

    return 0;
}
