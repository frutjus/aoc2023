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

ull possible(char* springs, char current_spring, ull *groups, ull current_group, char expect) {
    
    if (current_spring == '\0') {
        if (current_group != -1) {
            return 0; 
        }

        return 1;
        
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
        rec.springs = str;

        while (!peek(str, parse_spaces)) str++;
        *(str++) = '\0';
        
        rec.groups = parse_nums((const char**)&str, parse_comma);
        ArrayInt_push(&rec.groups, -1);

        parse_newline((const char**)&str);

        Array_push(records, rec);
    }

    ull ans = 0;

    iter(i, records.size) {
        printf("%s ", records.at[i].springs);
        
        iter(j, records.at[i].groups.size-1) {
            printf("%llu", records.at[i].groups.at[j]);
            if (j < records.at[i].groups.size - 2) {
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
