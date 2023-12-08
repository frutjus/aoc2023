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

int main() {
    String inp = read_file("inp/day8-sample3.txt");

    const char *str = inp.contents;

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

    Array(int) visits = Array_new(int);
    Array_push(visits, 1);
    struct { ull first_visit, next_visit; } ghost_visits[ghosts.size];

    iter(ghost, ghosts.size) {
        Array(int) next_visits = Array_new(int);
        ull first_visit = 0, next_visit = 0, step = 0;
        int first_visit_step;
        int first_visit_node;

        int current_node = ghosts.at[ghost];

        while (next_visit == 0) {
            Direction dir = dirs.at[step % dirs.size];
            current_node = hash(dir == left ? nodes[current_node].left : nodes[current_node].right);
            step++;

            if (current_node % 26 == 25) {
                if (first_visit == 0) {
                    first_visit = step;
                    first_visit_step = step % dirs.size;
                    first_visit_node = current_node;
                } else if (step % dirs.size == first_visit_step && current_node == first_visit_node) {
                    next_visit = step;
                }
            }
        }

        ghost_visits[ghost].first_visit = first_visit;
        ghost_visits[ghost].next_visit = next_visit;
    }

    iter(i, ghosts.size) {
        printf("fst = %llu; snd = %llu\n", ghost_visits[i].first_visit, ghost_visits[i].next_visit);
    }

    ull ans = 0;

    printf("Answer = %llu\n", ans);

    return 0;
}
