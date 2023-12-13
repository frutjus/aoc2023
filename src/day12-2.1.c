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

ull possible(char* springs, char current_spring, ull *groups, ull current_group, char expect) {
    
    if (current_spring == '\0') {
        if (current_group != -1) {
            return 0; 
        }

        return 1;
        
    }
    ull len = strlen(springs);
    ull space = minspace(groups, current_group);
    if (len < space) {
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

ull count_ways(Record rec) {
    return possible(rec.springs, *rec.springs, rec.groups.at, *rec.groups.at, '?');
}

int main() {
    char *str = read_file("inp/day12-sample.txt");

    Array(Record) records = Array_new(Record);

    while (*str != '\0') {
        Record rec;
        char *temp_str = str;
        int len = 0;

        while (!peek(str, parse_spaces)) {
            str++;
            len++;
        }
        str++;

        rec.springs = (char*)myalloc(len + 1);
        iter(k, len) {
            rec.springs[k] = temp_str[k];
        }
        rec.springs[len] = '\0';
        
        ArrayInt groups = parse_nums((const char**)&str, parse_comma);

        ull *new_groups = (ull*)myalloc(sizeof(ull) * groups.size + 1);
        iter(i, groups.size) {
            new_groups[i] = groups.at[i % groups.size];
        }
        new_groups[groups.size] = -1;

        free(groups.at);
        rec.groups.at = new_groups;
        rec.groups.size = groups.size;

        parse_newline((const char**)&str);

        Array_push(records, rec);
    }

    ull ans = 0;

    iter(i, records.size) {
        printf("%s ", records.at[i].springs);
        
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

    printf("Answer = [32m%llu[0m\n", ans);

    return 0;
}
