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
    String inp = read_file("inp/day8.txt");

    const char *str = inp.contents;

    Array(Direction) dirs = Array_new(Direction);

    while (!parse_newline(&str)) {
        Array_push(dirs, *(str++) == 'L' ? left : right);
    }

    parse_newline(&str);

    int nodeCount = 26 * 26 * 26;
    Pair nodes[nodeCount];

    while (*str != '\0') {
        int index = hash(str);
        str += 3;
        parse_string(&str, " = (");
        Pair pair;
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

    int current_node = 0;
    ull step = 0;

    while (current_node != hash("ZZZ")) {
        Direction dir = dirs.at[step % dirs.size];
        current_node = hash(dir == left ? nodes[current_node].left : nodes[current_node].right);
        step++;
    }

    printf("Answer = %llu\n", step);

    return 0;
}
