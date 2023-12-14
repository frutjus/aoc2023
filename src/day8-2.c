#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"

typedef enum {
    left,
    right,
} Direction;

int hash(const char name[3]) {
    return
    (name[0] - 'A') * 26 * 26 +
    (name[1] - 'A') * 26 +
    (name[2] - 'A');
}

typedef struct {
    char left[3];
    char right[3];
} Pair;

ull gcd(ull l, ull r) {
    ull divisor, dividend, remainder;
    if (l < r) {
        divisor = l;
        dividend = r;
    } else {
        divisor = r;
        dividend = l;
    }

    while ((remainder = dividend % divisor)) {
        dividend = divisor;
        divisor = remainder;
    }

    return divisor;
}

ull lcm(ull l, ull r) {
    return l * r / gcd(l, r);
}

int main() {
    const char *str = read_file("inp/day8.txt");

    Array(Direction) dirs = Array_new(Direction);

    while (!parse_newline(&str)) {
        Array_push(dirs, *(str++) == 'L' ? left : right);
    }

    parse_newline(&str);

    int nodeCount = 26 * 26 * 26;
    Pair nodes[nodeCount];
    Array(int) ghosts = Array_new(int);

    while (*str != '\0') {
        int index = hash(str);
        if (str[2] == 'A') {
            Array_push(ghosts, index);
        }
        Pair pair;
        str += 3;
        parse_string(&str, " = (");
        pair.left[0] = *(str++);
        pair.left[1] = *(str++);
        pair.left[2] = *(str++);
        parse_string(&str, ", ");
        pair.right[0] = *(str++);
        pair.right[1] = *(str++);
        pair.right[2] = *(str++);
        parse_string(&str, ")");
        parse_newline(&str);

        nodes[index] = pair;
    }

    ull ghost_visits[ghosts.size];

    // find the step at which each 'ghost' first visits an end node
    iter(ghost, ghosts.size) {
        ull first_visit = 0, step = 0;

        int current_node = ghosts.at[ghost];

        while (first_visit == 0) {
            Direction dir = dirs.at[step % dirs.size];
            current_node = hash(dir == left ? nodes[current_node].left : nodes[current_node].right);
            step++;

            if (current_node % 26 == 25) {
                first_visit = step;
            }
        }

        ghost_visits[ghost] = first_visit;
    }

    // This logic only works because every ghost takes exactly as long to get back to its end node as it took to get there the first time
    ull ans = ghost_visits[0];
    for (int i = 1; i < ghosts.size; i++) {
        ans = lcm(ans, ghost_visits[i]);
    }

    printf("Answer = %llu\n", ans);

    return 0;
}
