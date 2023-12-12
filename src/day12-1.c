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

bool check(Record rec) {
    char *spring = rec.springs;
    int group = 0;
    char expect = '?';
    ull current_group = rec.groups.at[0];

    while (*spring != '\0') {
        if (*spring == '#') {
            if (expect == '.') {
                return false;
            } else {
                current_group--;
                if (current_group > 0) {
                    expect = '#';
                } else {
                    expect = '.';
                }
            }
        } else if (*spring == '.') {
            if (expect == '#') {
                return false;
            } else if (current_group == 0) {
                group++;
                if (group >= rec.groups.size) {
                    expect = '.';
                } else {
                    expect = '?';
                    current_group = rec.groups.at[group];
                }
            }
        } else {
            printf("Unexpected character: |%d|\nInput: |%s|\nlen: %llu\n", *spring, rec.springs, strlen(rec.springs));
            exit(1);
        }

        spring++;
    }

    if (group < rec.groups.size - 1 || current_group != 0) {
        return false;
    } else {
        return true;
    }
}

ull count_ways(Record rec) {
    Array(int) qs = Array_new(int);

    for (int i = 0; *(rec.springs + i) != '\0'; i++) {
        if (*(rec.springs + i) == '?') {
            Array_push(qs, i);
        }
    }
    
    char new_springs[strlen(rec.springs)+1];
    memcpy(new_springs, rec.springs, strlen(rec.springs)+1);
    ull ways = 0;

    for (ull n = 0; n < ((ull)1 << qs.size); n++) {
        iter(i, qs.size) {
            new_springs[qs.at[i]] = n & ((ull)1 << i) ? '#' : '.';
        }

        if (check((Record) { .springs = new_springs, .groups = rec.groups })) {
            ways++;
        }
    }

    return ways;
}

int main() {
    String inp = read_file("inp/day12.txt");
    char *str = inp.contents;

    Array(Record) records = Array_new(Record);

    while (*str != '\0') {
        Record rec;
        rec.springs = str;

        while (!peek(str, parse_spaces)) str++;
        *(str++) = '\0';
        
        rec.groups = parse_nums((const char**)&str, parse_comma);

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
