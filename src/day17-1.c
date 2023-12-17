#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "array.h"

typedef struct {
    int r, c;
} Position;

int main() {
    const char *str = read_file("inp/day17-sample.txt");

    Grid grid = gridFromString(str);
    Grid visited = copyGrid(grid);
    zeroGrid(visited);
    ull ans = 0;

    iter(r, grid.height) {
        iter(c, grid.width) {
            grid.at[r][c] = charToDigit(grid.at[r][c]);
        }
    }

    printf("Answer = [32m%llu[0m\n", ans);

    return 0;
}
