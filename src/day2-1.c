#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "common.h"

int *parse_draw(const char **str) {
    static int colours[3];
    for (int i = 0; i < 3; i++) {
        colours[i] = 0;
    }

    do {
        int n = parse_num(str);
        parse_space(str);

        if (parse_string(str, "red")) colours[0] += n;
        else if (parse_string(str, "green")) colours[1] += n;
        else if (parse_string(str, "blue")) colours[2] += n;
    } while (parse_string(str, ", "));

    return colours;
}

int *parse_game(const char **str) {
    static int maxcolours[3];
    for (int i = 0; i < 3; i++) {
        maxcolours[i] = 0;
    }

    do {
        int *colours = parse_draw(str);

        for (int i = 0; i < 3; i++) {
            maxcolours[i] = colours[i] > maxcolours[i] ? colours[i] : maxcolours[i];
        }

    } while (parse_string(str, "; "));

    return maxcolours;
}

int main() {
    String inp = read_file("inp/day2.txt");

    int ans = 0;
    const char *str = inp.contents;
    int possibleColours[3] = { 12, 13, 14 };

    while (parse_string(&str, "Game ")) {
        int id = parse_num(&str);
        parse_string(&str, ": ");

        int *maxcolours = parse_game(&str);

        bool possible = true;

        for (int i = 0; i < 3; i++) {
            if (maxcolours[i] > possibleColours[i]) possible = false;
        }

        if (possible) ans += id;

        parse_newline(&str);
    }

    printf("Answer = %d\n", ans);

    return 0;
}