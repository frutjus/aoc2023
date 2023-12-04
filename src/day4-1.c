#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"

int main() {
    String inp = read_file("inp/day4.txt");

    int ans = 0;
    const char *str = inp.contents;

    while (parse_string(&str, "Card")) {

        // parse
        parse_spaces(&str);
        int id = parse_num(&str);

        parse_string(&str, ":");
        parse_spaces(&str);

        ArrayInt winners = parse_nums(&str, parse_spaces);

        parse_string(&str, "|");
        parse_spaces(&str);

        ArrayInt draws = parse_nums(&str, parse_spaces);

        parse_newline(&str);

        // solve

        int wins = 0;
        for (int i = 0; i < draws.size; i++) {
            if (ArrayInt_contains(winners, draws.contents[i])) {
                wins++;
            }
        }

        ans += wins == 0 ? 0 : pow(2, wins - 1);
        printf("Card %d had %d wins worth %.0f points\n", id, wins, wins == 0 ? 0 : pow(2, wins - 1));
    }
    
    printf("Answer = %d\n", ans);

    return 0;
}