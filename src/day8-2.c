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
    char name[3];
    char left[3];
    char right[3];
} Node;

bool atend(int *ghosts, size_t len, Node *nodes) {
    iter(i, len) {
        if (nodes[ghosts[i]].name[2] != 'Z') {
            return false;
        }
    }
    return true;
}

int main() {
    String inp = read_file("inp/day8.txt");

    const char *str = inp.contents;

    Array(Direction) dirs = Array_new(Direction);

    while (!parse_newline(&str)) {
        Array_push(dirs, *(str++) == 'L' ? left : right);
    }

    parse_newline(&str);

    int nodeCount = 26 * 26 * 26;
    Node nodes[nodeCount];
    Array(int) ghosts = Array_new(int);

    while (*str != '\0') {
        int index = hash(str);
        if (str[2] == 'A') {
            Array_push(ghosts, index);
        }
        Node pair;
        pair.name[0] = *(str++);
        pair.name[1] = *(str++);
        pair.name[2] = *(str++);
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

    ull step = 0;

    while (!atend(ghosts.at, ghosts.size, nodes)) {
        if (step % 1000000 == 0) {
            printf("Step %llu: ", step);
            iter(i, ghosts.size) {
                printf("%c%c%c ", nodes[ghosts.at[i]].name[0], nodes[ghosts.at[i]].name[1], nodes[ghosts.at[i]].name[2]);
            }
            printf("\n");
        }
        Direction dir = dirs.at[step % dirs.size];
        iter(i, ghosts.size) {
            ghosts.at[i] = hash(dir == left ? nodes[ghosts.at[i]].left : nodes[ghosts.at[i]].right);
        }
        step++;
    }

    printf("Answer = %llu\n", step);

    return 0;
}
