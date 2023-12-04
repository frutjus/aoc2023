#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"

int main() {
    String inp = read_file("inp/day4.txt");

    int ans = 0;
    const char *str = inp.contents;

    int numCards = countLines(str);
    printf("Cards = %d\n", numCards);
    int copies[numCards];

    for (int i = 0; i < numCards; i++) {
        copies[i] = 1;
    }

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

        for (int i = id; i < id + wins; i++) {
            copies[i] += copies[id-1];
        }

        ans += copies[id-1];

        printf("Card %d had %d copies\n", id, copies[id-1]);
    }
    
    printf("Answer = %d\n", ans);

    return 0;
}