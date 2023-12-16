#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"

typedef struct {
    char *springs;
    ArrayInt groups;
} Record;

ull minspace(ull *groups, ull current_group) {
    ull sum = 0;

    if (current_group == -1) {
        return 0;
    }

    while (current_group != -1) {
        sum += current_group + 1;
        current_group = *(++groups);
    }

    return sum - 1;
}

typedef struct {
    char *springs;
    char current_spring;
    ull *groups;
    ull current_group;
    char expect;
} ArgTuple;

typedef struct _List {
    ArgTuple key;
    ull value;
    struct _List *tail;
} List;

List List_new() {
    return (List) { .key = { 0 }, .value = 0, .tail = 0 };
}

List *List_newm() {
    List *list = (List*)myalloc(sizeof(List));
    *list = List_new();
    return list;
}

bool List_isempty(List list) {
    return list.tail == 0;
}

bool ArgTuple_equal(ArgTuple l, ArgTuple r) {
    
    if (l.springs != r.springs /* && strcmp(l.springs + 1, r.springs + 1) != 0 */) {
        return false;
    }

    if (l.current_spring != r.current_spring) {
        return false;
    }

    if (l.groups != r.groups) {
        /* while (*(++l.groups) != -1 || *(++r.groups) != -1) {
            if (*l.groups != *r.groups) {
                return false;
            }
        } */
        return false;
    }

    if (l.current_group != r.current_group) {
        return false;
    }

    if (l.expect != r.expect) {
        return false;
    }

    return true;
}

ull List_insert(List *list, ArgTuple key, ull f(ArgTuple)) {
    if (List_isempty(*list)) {
        List *tail = List_newm();
        list->key = key;
        list->value = f(key);
        list->tail = tail;
        return list->value;
    } else if (ArgTuple_equal(list->key, key)) {
        return list->value;
    } else {
        return List_insert(list->tail, key, f);
    }
}

ull List_size(List list) {
    if (List_isempty(list)) {
        return 0;
    } else {
        return 1 + List_size(*list.tail);
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

int hash(ArgTuple *key) {
    int current_value = 0;

    iter(i, sizeof(ArgTuple)) {
        current_value = ((current_value + *((char*)key + i)) * 17) % MAP_BUCKETS;
    }

    return current_value;
}

ull Map_insert(Hashmap *map, ArgTuple key, ull f(ArgTuple)) {
    int keyhash = hash(&key);

    return List_insert(&map->buckets[keyhash], key, f);
}

ull Map_size(Hashmap *map) {
    ull size = 0;
    iter(i, MAP_BUCKETS) {
        size += List_size(map->buckets[i]);
    }
    return size;
}

Hashmap memo;

ull possible(ArgTuple args);

ull _possible(ArgTuple args) {
    
    if (args.current_spring == '\0') {
        if (args.current_group != -1) {
            return 0; 
        }

        return 1;
        
    }
    if (strlen(args.springs) < minspace(args.groups, args.current_group)) {
        return 0;
    }
    if (args.current_spring == '#') {
        if (args.expect == '.') {
            return 0;
        }
        if (args.current_group == -1) {
            return 0;
        }

        args.current_group--;

        if (args.current_group == 0) {
            args.expect = '.';
            args.groups++;
            args.current_group = *args.groups;
        } else {
            args.expect = '#';
        }

        return possible((ArgTuple) { args.springs + 1, *(args.springs + 1), args.groups, args.current_group, args.expect});

    }
    if (args.current_spring == '.') {
        if (args.expect == '#') {
            return 0;
        }

        args.expect = '?';
        return possible((ArgTuple) { args.springs + 1, *(args.springs + 1), args.groups, args.current_group, args.expect});

    }
    if (args.current_spring == '?') {
        return possible((ArgTuple) { args.springs, '#', args.groups, args.current_group, args.expect })
             + possible((ArgTuple) { args.springs, '.', args.groups, args.current_group, args.expect });
    }
    
    printf("Unrecognised spring: %c\n", *args.springs);
    exit(1);
}

ull possible(ArgTuple args) {
    return Map_insert(&memo, args, _possible);
}

ull count_ways(Record rec) {
    return possible((ArgTuple) { rec.springs, *rec.springs, rec.groups.at, *rec.groups.at, '?' });
}

int main() {
    char *str = read_file("inp/day12.txt");

    Array(Record) records = Array_new(Record);

    memo = Map_new();

    while (*str != '\0') {
        Record rec;
        char *temp_str = str;
        int len = 0;

        while (!peek(str, parse_spaces)) {
            str++;
            len++;
        }
        str++;

        rec.springs = (char*)myalloc(len * 5 + 4 + 1);
        int j = 0;
        iter(i, 5) {
            iter(k, len) {
                rec.springs[j++] = temp_str[k];
            }
            if (i < 4) {
                rec.springs[j++] = '?';
            }
        }
        rec.springs[j] = '\0';
        
        ArrayInt groups = parse_nums((const char**)&str, parse_comma);

        ull *new_groups = (ull*)myalloc(sizeof(ull) * groups.size * 5 + 1);
        iter(i, groups.size * 5) {
            new_groups[i] = groups.at[i % groups.size];
        }
        new_groups[groups.size * 5] = -1;

        free(groups.at);
        rec.groups.at = new_groups;
        rec.groups.size = groups.size * 5;

        parse_newline((const char**)&str);

        Array_push(records, rec);
    }

    ull ans = 0;

    iter(i, records.size) {
        printf("%4d:  %s ", i, records.at[i].springs);
        
        iter(j, records.at[i].groups.size) {
            printf("%llu", records.at[i].groups.at[j]);
            if (j < records.at[i].groups.size - 1) {
                printf(",");
            }
        }

        ull ways = count_ways(records.at[i]);
        ans += ways;

        printf(" [32m%llu[0m\n", ways);
    }

    printf("Answer = [32m%llu[0m\nMemo size = %llu\n", ans, Map_size(&memo));

    return 0;
}
