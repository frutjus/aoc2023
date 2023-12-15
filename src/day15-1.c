#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"

int main() {
    char *str = read_file("inp/day15.txt");

    ull ans = 0;

    while (*str != '\0' && !parse_newline((const char**)&str)) {
        int current_value = 0;

        while (!parse_comma((const char**)&str) && !parse_newline((const char**)&str)) {
            current_value = ((current_value + *str) * 17) % 256;
            str++;
        }

        ans += current_value;
    }

    printf("Answer = [32m%llu[0m\n", ans);

    return 0;
}
