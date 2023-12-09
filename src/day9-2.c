#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"

typedef Array(long long int) ArrI;

long long int sum(ArrI arr) {
    long long int acc = 0;
    iter(i, arr.size) {
        acc += arr.at[i];
    }
    return acc;
}

bool allzero(ArrI arr) {
    iter(i, arr.size) {
        if (arr.at[i]) return false;
    }
    return true;
}

long long int estimate(ArrI history) {
    Array(ArrI) poly = Array_new(ArrI);

    Array_push(poly, history);

    while (!allzero(Array_last(poly))) {
        ArrI diffs = Array_new(long long int);

        iter(i, Array_last(poly).size - 1) {
            Array_push(diffs, Array_last(poly).at[i+1] - Array_last(poly).at[i]);
        }

        Array_push(poly, diffs);
    }

    for (int i = poly.size - 1; i --> 0;) {
        long long int diff = Array_last(poly.at[i+1]);

        long long int next = Array_last(poly.at[i]) + diff;

        Array_push(poly.at[i], next);
    }

    return Array_last(poly.at[0]);
}

int main() {
    String inp = read_file("inp/day9.txt");

    const char *str = inp.contents;

    ArrI estimates = Array_new(long long int);

    while (*str != '\0') {
        ArrI history = Array_new(long long int);

        do {
            Array_push(history, parse_num(&str));
        } while (parse_spaces(&str));
        parse_newline(&str);

        Array_reverse(long long int, history);

        iter(i, history.size) {
            printf("%lld ", history.at[i]);
        }

        Array_push(estimates, estimate(history));

        printf("[32m%lld[0m\n", Array_last(estimates));
    }

    long long int ans = sum(estimates);

    printf("Answer = %llu\n", ans);

    return 0;
}
