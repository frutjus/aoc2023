#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"
#include "list.h"

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
    ull result;
} ArgTuple;

Array(ArgTuple) memo;

bool ArgTuple_equal(ArgTuple l, ArgTuple r) {
    
    if (l.springs != r.springs && strcmp(l.springs + 1, r.springs + 1) == 0) {
        return false;
    }

    if (l.current_spring != r.current_spring) {
        return false;
    }

    if (l.groups != r.groups) {
        while (*(++l.groups) != -1 || *(++r.groups) != -1) {
            if (*l.groups != *r.groups) {
                return false;
            }
        }
    }

    if (l.current_group != r.current_group) {
        return false;
    }

    if (l.expect != r.expect) {
        return false;
    }

    return true;
}

ull possible(char* springs, char current_spring, ull *groups, ull current_group, char expect);

ull _possible(char* springs, char current_spring, ull *groups, ull current_group, char expect) {
    
    if (current_spring == '\0') {
        if (current_group != -1) {
            return 0; 
        }

        return 1;
        
    }
    if (strlen(springs) < minspace(groups, current_group)) {
        return 0;
    }
    if (current_spring == '#') {
        if (expect == '.') {
            return 0;
        }
        if (current_group == -1) {
            return 0;
        }

        current_group--;

        if (current_group == 0) {
            expect = '.';
            groups++;
            current_group = *groups;
        } else {
            expect = '#';
        }

        return possible(springs + 1, *(springs + 1), groups, current_group, expect);

    }
    if (current_spring == '.') {
        if (expect == '#') {
            return 0;
        }

        expect = '?';
        return possible(springs + 1, *(springs + 1), groups, current_group, expect);

    }
    if (current_spring == '?') {
        return possible(springs, '#', groups, current_group, expect)
             + possible(springs, '.', groups, current_group, expect);
    }
    
    printf("Unrecognised spring: %c\n", *springs);
    exit(1);
}

ull possible(char* springs, char current_spring, ull *groups, ull current_group, char expect) {
    ArgTuple args = { springs, current_spring, groups, current_group, expect };

    iter(i, memo.size) {
        if (ArgTuple_equal(memo.at[i], args)) {
            return memo.at[i].result;
        }
    }

    args.result = _possible(springs, current_spring, groups, current_group, expect);
    Array_push(memo, args);
    
    printf("New result: springs=%s; current_spring=%c; groups=", springs, current_spring);
    while (*groups != -1) {
        printf("%llu,", *groups);
        groups++;
    }
    printf(" current_group=%llu; expect=%c; result=%llu\n", current_group, expect, args.result);

    return args.result;
}

ull count_ways(Record rec) {
    return possible(rec.springs, *rec.springs, rec.groups.at, *rec.groups.at, '?');
}

int main() {
    char *str = read_file("inp/day12-sample.txt");

    Array(Record) records = Array_new(Record);

    memo.at = (ArgTuple*)myalloc(sizeof(ArgTuple) * ARRAY_MIN_SIZE);
    memo.size = 0;
    memo.capacity = ARRAY_MIN_SIZE;

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

    iter(i, records.size) if (i == 1) {
        printf("%s ", records.at[i].springs);
        
        iter(j, records.at[i].groups.size) {
            printf("%llu", records.at[i].groups.at[j]);
            if (j < records.at[i].groups.size - 1) {
                printf(",");
            }
        }
        printf("\n");

        ull ways = count_ways(records.at[i]);
        ans += ways;

        printf("[32m%llu[0m\n", ways);
    }

    //printf("Answer = [32m%llu[0m\n", ans);

    return 0;
}
