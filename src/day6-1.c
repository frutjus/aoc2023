#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"

int main() {
    String inp = read_file("inp/day6.txt");

    const char *str = inp.contents;

    parse_string(&str, "Time:");
    parse_spaces(&str);
    ArrayInt times = parse_nums(&str, parse_spaces);
    parse_newline(&str);
    parse_string(&str, "Distance:");
    parse_spaces(&str);
    ArrayInt distances = parse_nums(&str, parse_spaces);
    parse_newline(&str);
    
    ull ans = 1;
    
    for (int i = 0; i < times.size; i++) {
        ull ways = 0;

        for (int j = 1; j < times.contents[i]; j++) {
            if (j * (times.contents[i] - j) > distances.contents[i]) {
                ways++;
            }
        }

        ans *= ways;
    }
    
    printf("Answer = %llu\n", ans);

    return 0;
}
