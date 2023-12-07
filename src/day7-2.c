#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"

typedef enum {
    highcard,
    onepair,
    twopair,
    threeofakind,
    fullhouse,
    fourofakind,
    fiveofakind,
} Type;

typedef struct {
    char hand[5];
    ull bid;
} Entry;

#define iter(i, n) for (int i = 0; i < n; i++)

Type determine_type1(char hand[5]) {
    // count the number of each card that appears in the hand
    int count[5] = { 0 };

    iter(i, 5) {
        iter(j, 5) {
            if (hand[j] == hand[i]) {
                count[i]++;
            }
        }
    }

    // count the number of runs of each possible length (1-5)
    int run[6] = { 0 };

    iter(i, 5) {
        run[count[i]]++;
    }

    // use the run counts to classify the type
    if (run[5]) return fiveofakind;
    if (run[4]) return fourofakind;
    if (run[3] && run[2]) return fullhouse;
    if (run[3]) return threeofakind;
    if (run[2] == 4) return twopair;
    if (run[2]) return onepair;
    return highcard;
}

Type determine_type(char hand[5]) {
    Type best = highcard;
    char newhand[5];
    char allcards[12] = "AKQT98765432";

    iter(card, 12) {
        iter(i, 5) {
            newhand[i] = hand[i] == 'J' ? allcards[card] : hand[i];
        }
        Type type = determine_type1(newhand);
        best = type > best ? type : best;
    }

    return best;
}

int strength(char card) {
    switch (card) {
        case 'A': return 13;
        case 'K': return 12;
        case 'Q': return 11;
        case 'J': return 0;
        case 'T': return 9;
        case '9': return 8;
        case '8': return 7;
        case '7': return 6;
        case '6': return 5;
        case '5': return 4;
        case '4': return 3;
        case '3': return 2;
        case '2': return 1;
        default:
            printf("Huh... We are not supposed to have a card like '%c'...\n", card);
            return -1;
    }
}

int cmp_entry(Entry e1, Entry e2) {
    int relative_strength = determine_type(e1.hand) - determine_type(e2.hand);

    if (relative_strength > 0) return 1;
    if (relative_strength < 0) return -1;
    
    iter(i, 5) {
        relative_strength = strength(e1.hand[i]) - strength(e2.hand[i]);
        if (relative_strength > 0) return 1;
        if (relative_strength < 0) return -1;
    }

    return 0;
}

int main() {
    String inp = read_file("inp/day7.txt");

    const char *str = inp.contents;

    Array(Entry) entries = Array_new(Entry);

    while (*str != '\0') {
        Array_push(entries, (Entry) {});
        for (int i = 0; i < 5; i++) {
            Array_last(entries).hand[i] = *(str++);
        }
        parse_spaces(&str);
        Array_last(entries).bid = parse_num(&str);
        parse_newline(&str);
    }

    Array_sort(Entry, entries, cmp_entry);

    ull ans = 0;
    
    iter(i, entries.size) {
        ans += entries.at[i].bid * (i + 1);
        printf("%4d | ", i);
        iter(j, 5) printf("%c", entries.at[i].hand[j]);
        printf(" type=%d\n", determine_type(entries.at[i].hand));
    }

    printf("Answer = %llu\n", ans);

    return 0;
}
