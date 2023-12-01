#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "common.h"

typedef struct {
    const char str[6];
    const int num;
    const int len;
} Representation;

const Representation reps[] = {
    { "1",      1, 1 },
    { "2",      2, 1 },
    { "3",      3, 1 },
    { "4",      4, 1 },
    { "5",      5, 1 },
    { "6",      6, 1 },
    { "7",      7, 1 },
    { "8",      8, 1 },
    { "9",      9, 1 },
    { "one",    1, 3 },
    { "two",    2, 3 },
    { "three",  3, 5 },
    { "four",   4, 4 },
    { "five",   5, 4 },
    { "six",    6, 3 },
    { "seven",  7, 5 },
    { "eight",  8, 5 },
    { "nine",   9, 4 },
};
const size_t numReps = sizeof(reps) / sizeof(Representation);

Representation checkNum(char *str) {
    for (int i = 0; i < numReps; i++) {
        if (isPrefixOf(reps[i].str, str)) return reps[i];
    }
    return (Representation) { "", -1, 0 };
}

int main() {
    String inp = read_file("inp/day1.txt");

    int first = -1, last = -1, ans = 0;

    for (int i = 0; i < inp.size; i++) {
        Representation rep = checkNum(inp.contents + i);

        if (rep.num != -1) {
            if (first == -1) {
                first = rep.num;
            }
            last = rep.num;
            printf("[32m%c[0m", inp.contents[i]);
            //i += rep.len - 1;
        } else {
            if (inp.contents[i] == '\n') {
                int value = first * 10 + last;
                ans += value;
                printf(" | value = %d", value);
                first = -1;
            }
            printf("[0m%c[0m", inp.contents[i]);
        }
    }

    printf("Answer = %d\n", ans);

    return 0;
}