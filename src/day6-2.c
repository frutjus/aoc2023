#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"

ull parse_num_spaced(const char **str) {
    ull num = 0;

    while (!parse_newline(str)) {
        if (isdigit(**str)) {
            num = num * 10 + charToDigit(**str);
        }
        (*str)++;
    }

    return num;
}

int main() {
    String inp = read_file("inp/day6.txt");

    const char *str = inp.contents;

    parse_string(&str, "Time:");
    parse_spaces(&str);
    ull time = parse_num_spaced(&str);
    parse_string(&str, "Distance:");
    parse_spaces(&str);
    ull distance = parse_num_spaced(&str);

    printf("Time = %llu; Distance = %llu\n", time, distance);
    
    ull ans = 0;
    
    for (int j = 1; j < time; j++) {
        if (j * (time - j) > distance) {
            ans++;
        }
    }

    printf("Answer = %llu\n", ans);

    return 0;
}
